#include "ScriptEngine.h"

#include <filesystem>
#include <iostream>
#include <regex>
#include <vector>

#include "../Loggers/LoggerSingleton.h"

#ifndef SCRIPT_ENGINE_ASSEMBLY_NAME
#define SCRIPT_ENGINE_ASSEMBLY_NAME "DreamEngine"
#endif

#ifndef SCRIPT_ENGINE_SCRIPT_MANAGER_NAME
#define SCRIPT_ENGINE_SCRIPT_MANAGER_NAME "DreamEngine.Managers.ScriptManager"
#endif

using namespace DreamEngine::Core::Scripting;
using namespace DreamEngine::Core::Loggers;

void* ScriptEngine::m_spHostHandle = nullptr;
unsigned int ScriptEngine::m_sDomainId = 0;
// coreclr
coreclr_initialize_ptr ScriptEngine::m_spCoreClrInitialize = nullptr;
coreclr_create_delegate_ptr ScriptEngine::m_spCoreClrCreateDelegate = nullptr;
coreclr_shutdown_ptr ScriptEngine::m_spCoreClrShutdown = nullptr;
// scripts delegate
updateDelegate ScriptEngine::m_spUpdateDelegate = nullptr;
createInstanceDelegate ScriptEngine::m_spCreateInstanceDelegate = nullptr;
releaseInstanceDelegate ScriptEngine::m_spReleaseInstanceDelegate = nullptr;
getScriptInfoDelegate ScriptEngine::m_spGetScriptInfoDelegate = nullptr;
releaseScriptInfoDelegate ScriptEngine::m_spReleaseScriptInfoDelegate = nullptr;
updateGameDelegate ScriptEngine::m_spUpdateGameDelegate = nullptr;
processEventDelegate ScriptEngine::m_spProcessEventDelegate = nullptr;
// assembly delegate
assemblyInitializeDelegate ScriptEngine::m_spAssemblyInitializeDelegate = nullptr;
assembliesUnloadDelegate ScriptEngine::m_spAssembliesUnloadDelegate = nullptr;

ScriptEngine::ScriptEngine()
{
    Initialize();
}

ScriptEngine::~ScriptEngine() = default;

bool ScriptEngine::Initialize()
{
    m_coreClrPath = FindCoreCLRPath();

    if (!LoadCoreCLR(m_coreClrPath))
        return false;

    if (!InitCoreCLR())
        return false;

    return CreateCoreCLRDelegates();
}

std::string ScriptEngine::FindCoreCLRPath()
{
    // 1. Check environment variable
    const char* envPath = std::getenv("CORECLR_PATH");

    if (envPath && std::filesystem::exists(envPath))
    {
        std::string envPathStr = std::string(envPath);
        LoggerSingleton::Instance().LogTrace("Using CORECLR_PATH from environment: " + envPathStr);
        return envPathStr;
    }

#ifdef _WIN32
    std::string basePath = "C:/Program Files/dotnet/shared/Microsoft.NETCore.App/";
#else
    std::string basePath = "/usr/share/dotnet/shared/Microsoft.NETCore.App/";
#endif

    if (!std::filesystem::exists(basePath))
        throw std::runtime_error("Can't find CoreCLR base directory!");

    if (std::filesystem::exists(basePath))
    {
        std::string latest;
        for (auto& entry : std::filesystem::directory_iterator(basePath))
        {
            if (entry.is_directory())
            {
                latest = entry.path().string();  // Just pick the last for now
            }
        }
        if (!latest.empty())
        {
            LoggerSingleton::Instance().LogTrace("Using latest detected CoreCLR version: " + latest);
            return NormalizePath(latest);
        }
    }

    throw std::exception("Can't find CoreCLR!");
}

void ScriptEngine::LoadAssembly(const std::string& assemblyPath)
{
    if (!m_isAssemblyLoaded)
    {
        m_spAssemblyInitializeDelegate(assemblyPath.c_str());
        m_isAssemblyLoaded = true;
        LoggerSingleton::Instance().LogInfo("ScriptEngine::LoadAssembly -> Assembly loaded successfully!");
    }
    else
        LoggerSingleton::Instance().LogWarning("ScriptEngine::LoadAssembly -> The Script Engine already have assembly loaded!");
}

void ScriptEngine::UnloadAssembly()
{
    if (m_isAssemblyLoaded)
    {
        m_spAssembliesUnloadDelegate();
        m_isAssemblyLoaded = false;
    }
    else
        LoggerSingleton::Instance().LogWarning("ScriptEngine::UnloadAssembly -> The Script Engine does not have any assembly loaded!");
}

void ScriptEngine::Shutdown()
{
    LoggerSingleton::Instance().LogTrace("ScriptEngine::Shutdown -> start");

    // Shutdown CoreCLR
    int shutdownStatus = m_spCoreClrShutdown(m_spHostHandle, m_sDomainId);

    if (shutdownStatus != 0)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::Initialize -> CoreCLR shutdown failed with error code: ");
        return;
    }

    m_spHostHandle = nullptr;
    m_sDomainId = 0;

    // delegates
    m_spUpdateDelegate = nullptr;
    m_spCreateInstanceDelegate = nullptr;
    m_spReleaseInstanceDelegate = nullptr;
    m_spGetScriptInfoDelegate = nullptr;
}

bool ScriptEngine::IsRunning()
{
    return m_spHostHandle != nullptr;
}

void* ScriptEngine::CreateInstance(const char* assemblyName, const char* typeName)
{
    void* pointer = m_spCreateInstanceDelegate(assemblyName, typeName);

    return pointer;
}

void ScriptEngine::ReleaseInstance(void* instance)
{
    m_spReleaseInstanceDelegate(instance);
}

void ScriptEngine::Update(void* instance, void* entityData)
{
    m_spUpdateDelegate(instance, entityData);
}

void ScriptEngine::UpdateGame(void* gameData)
{
    m_spUpdateGameDelegate(gameData);
}

void ScriptEngine::ProcessEvent(const int eventId)
{
    m_spProcessEventDelegate(eventId);
}

std::vector<ScriptInfo> ScriptEngine::GetClassInfoList()
{
    LoggerSingleton::Instance().LogTrace("ScriptEngine::GetClassInfoList -> Start");

    std::vector<ScriptInfo> scriptInfo;

    if (!IsRunning())
    {
        LoggerSingleton::Instance().LogWarning("Script Engine is not running!");

        return scriptInfo;
    }

    int count = 0;
    std::vector<ScriptInfo> scriptInfos;
    size_t structSize = sizeof(ScriptInfo);

    void* result = m_spGetScriptInfoDelegate(&count);

    if (result == nullptr || count == 0)
    {
        LoggerSingleton::Instance().LogWarning("No classes found inheriting from Script!");

        return scriptInfo;
    }

    for (int i = 0; i < count; i++)
    {
        ScriptInfo* scriptInfo = reinterpret_cast<ScriptInfo*>(reinterpret_cast<char*>(result) + i * structSize);
        scriptInfos.push_back({scriptInfo->AssemblyName, scriptInfo->Name});
    }

    // FIXME: This is causing a crash
    //m_spReleaseScriptInfoDelegate(&result, &count);

    return scriptInfos;
}

std::string ScriptEngine::BuildTpaList(const std::string& coreClrPath)
{
    std::string tpaList;

    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(coreClrPath))
        {
            if (entry.is_regular_file() && entry.path().extension() == ".dll")
            {
                if (tpaList.empty())
                    tpaList += entry.path().string();
                else
                    tpaList += ";" + entry.path().string();
            }
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cerr << "Error accessing directory: " << e.what() << '\n';
    }

    return tpaList;
}

bool ScriptEngine::LoadCoreCLR(const std::string& coreClrPath)
{
    LoggerSingleton::Instance().LogTrace("ScriptEngine::LoadCoreCLR -> Start");

    m_pCoreClrLibrary = LoadLibrary((coreClrPath + "\\coreclr.dll").c_str());

    if (!m_pCoreClrLibrary)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::LoadCoreCLR -> Failed to load CoreCLR.");

        return false;
    }

     // Load CoreCLR APIs
    m_spCoreClrInitialize = reinterpret_cast<coreclr_initialize_ptr>(GetFunction(m_pCoreClrLibrary, "coreclr_initialize"));
    m_spCoreClrCreateDelegate = reinterpret_cast<coreclr_create_delegate_ptr>(GetFunction(m_pCoreClrLibrary, "coreclr_create_delegate"));
    m_spCoreClrShutdown = reinterpret_cast<coreclr_shutdown_ptr>(GetFunction(m_pCoreClrLibrary, "coreclr_shutdown"));

    if (!m_spCoreClrInitialize || !m_spCoreClrCreateDelegate || !m_spCoreClrShutdown)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::LoadCoreCLR -> Failed to load CoreCLR APIs");

        return false;
    }

    return true;
}

bool ScriptEngine::InitCoreCLR()
{
    LoggerSingleton::Instance().LogTrace("ScriptEngine::InitCoreCLR -> Start");

    std::string executablePath = GetExecutablePath(true);
    std::string dreamEnginePath = executablePath + "\\" + "DreamEngine.dll";

    // Paths for CoreCLR
    std::string tpaPaths = BuildTpaList(m_coreClrPath) + ";" + dreamEnginePath;
    std::string nativePath = executablePath + "\\" + "Core.dll";

    // Property keys and values
    std::vector<const char*> propertyKeys = 
    {
        "TRUSTED_PLATFORM_ASSEMBLIES",
        "APP_PATHS",
        "NATIVE_DLL_SEARCH_DIRECTORIES"
    };
    std::vector<const char*> propertyValues = 
    {
        tpaPaths.c_str(),
        executablePath.c_str(),
        nativePath.c_str()
    };

    // Get pointers to the keys and values
    const char** keysPtr = propertyKeys.data();
    const char** valuesPtr = propertyValues.data();
    int propertyCount = static_cast<int>(propertyKeys.size());

    // Initialize CoreCLR
    std::string exePath = GetExecutablePath();
    int initStatus = m_spCoreClrInitialize(
        exePath.c_str(),              // Path to your C++ executable
        "DreamEngineDomain",          // Friendly name for the AppDomain
        propertyCount,                // Number of properties
        keysPtr,                      // Pointer to keys
        valuesPtr,                    // Pointer to values
        &m_spHostHandle,              // [OUT] Handle to the runtime
        &m_sDomainId                  // [OUT] AppDomain ID
    );

    keysPtr = nullptr;
    valuesPtr = nullptr;
    propertyKeys.clear();
    propertyValues.clear();

    if (initStatus != 0)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::Initialize -> Failed to initialize CoreCLR. " + std::to_string(initStatus));

        return false;
    }

    LoggerSingleton::Instance().LogInfo("CoreCLR initialized successfully!");
}

std::string ScriptEngine::NormalizePath(const std::string& path)
{
#ifdef _WIN32
    std::string normalized = path;
    std::replace(normalized.begin(), normalized.end(), '/', '\\');
    return normalized;
#else
    return path;
#endif
}

bool ScriptEngine::CreateCoreCLRDelegates()
{
    LoggerSingleton::Instance().LogTrace("ScriptEngine::CreateCoreCLRDelegates -> Start");

    // Create instance delegate
    const int createInstanceDelegateStatus = m_spCoreClrCreateDelegate(
        m_spHostHandle,                      // CoreCLR host handle
        m_sDomainId,                         // AppDomain ID
        SCRIPT_ENGINE_ASSEMBLY_NAME,         // Assembly name
        SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,   // Type name (including namespace)
        "CreateInstance",                    // Method name
        (void**)&m_spCreateInstanceDelegate  // Delegate to store the function pointer
    );

    if (createInstanceDelegateStatus != 0)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::CreateCoreCLRDelegates -> Failed to bind CreateInstance delegate.");
    }
    
    // Release delegate
    const int releaseInstanceDelegateStatus = m_spCoreClrCreateDelegate(
        m_spHostHandle,                       // CoreCLR host handle
        m_sDomainId,                          // AppDomain ID
        SCRIPT_ENGINE_ASSEMBLY_NAME,          // Assembly name
        SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,    // Type name (including namespace)
        "ReleaseInstance",                    // Method name
        (void**)&m_spReleaseInstanceDelegate  // Delegate to store the function pointer
    );

    if (releaseInstanceDelegateStatus != 0)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::CreateCoreCLRDelegates -> Failed to bind ReleaseInstance delegate.");
    }


    // Update delegate
    const int updateDelegateStatus = m_spCoreClrCreateDelegate(
        m_spHostHandle,                     // CoreCLR host handle
        m_sDomainId,                        // AppDomain ID
        SCRIPT_ENGINE_ASSEMBLY_NAME,        // Assembly name
        SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,  // Type name (including namespace)
        "UpdateInstance",                   // Method name
        (void**)&m_spUpdateDelegate         // Delegate to store the function pointer
    );
    
    if (updateDelegateStatus != 0)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::CreateCoreCLRDelegates -> Failed to bind Update delegate.");
    }
    
    const int getScriptInfoStatus = m_spCoreClrCreateDelegate(
        m_spHostHandle,                     // CoreCLR host handle
        m_sDomainId,                        // AppDomain ID
        SCRIPT_ENGINE_ASSEMBLY_NAME,        // Assembly name
        SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,  // Type name (including namespace)
        "GetScriptInfo",                    // Method name
        (void**)&m_spGetScriptInfoDelegate   // Delegate to store the function pointer
    );

    if (getScriptInfoStatus != 0)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::CreateCoreCLRDelegates -> Failed to bind GetScriptInfo delegate.");
    }

    const int updateGameStatus = m_spCoreClrCreateDelegate(
        m_spHostHandle,                     // CoreCLR host handle
        m_sDomainId,                        // AppDomain ID
        SCRIPT_ENGINE_ASSEMBLY_NAME,        // Assembly name
        SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,  // Type name (including namespace)
        "UpdateGame",                      // Method name
        (void**)&m_spUpdateGameDelegate   // Delegate to store the function pointer
    );

    if (updateGameStatus != 0)
    {
        LoggerSingleton::Instance().LogError("ScriptEngine::CreateCoreCLRDelegates -> Failed to bind UpdateGame delegate.");
    }

    const int assemblyInitStatus = m_spCoreClrCreateDelegate(
		m_spHostHandle,							// CoreCLR host handle
		m_sDomainId,							// AppDomain ID
		SCRIPT_ENGINE_ASSEMBLY_NAME,			// Assembly name
		SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,		// Type name (including namespace)
		"AssemblyInitialize",					// Method name
		(void**)&m_spAssemblyInitializeDelegate	// Delegate to store the function pointer
	);
    
	if (assemblyInitStatus != 0)
	{
        LoggerSingleton::Instance().LogError("Failed to bind AssemblyInitialize delegate. ");
	}

	const int assemblyUnloadStatus = m_spCoreClrCreateDelegate(
		m_spHostHandle,							// CoreCLR host handle
		m_sDomainId,							// AppDomain ID
		SCRIPT_ENGINE_ASSEMBLY_NAME,			// Assembly name
		SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,		// Type name (including namespace)
		"AssembliesUnload",						// Method name
		(void**)&m_spAssembliesUnloadDelegate	// Delegate to store the function pointer
	);
    
	if (assemblyUnloadStatus != 0)
	{
        LoggerSingleton::Instance().LogError("Failed to bind AssembliesUnload delegate. ");
	}

    const int releaseScriptInfoStatus = m_spCoreClrCreateDelegate(
		m_spHostHandle,                     // CoreCLR host handle
		m_sDomainId,                        // AppDomain ID
		SCRIPT_ENGINE_ASSEMBLY_NAME,        // Assembly name
		SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,  // Type name (including namespace)
		"ReleaseScriptInfos",               // Method name
		(void**)&m_spReleaseScriptInfoDelegate   // Delegate to store the function pointer
	);

	if (releaseScriptInfoStatus != 0)
	{
        LoggerSingleton::Instance().LogError("Failed to bind ReleaseScriptInfos delegate.");
		return false;
	}

    const int processEventStatus = m_spCoreClrCreateDelegate(m_spHostHandle,                         // CoreCLR host handle
         m_sDomainId,                            // AppDomain ID
         SCRIPT_ENGINE_ASSEMBLY_NAME,            // Assembly name
         SCRIPT_ENGINE_SCRIPT_MANAGER_NAME,      // Type name (including namespace)
         "ProcessEvent",                         // Method name
         (void**)&m_spProcessEventDelegate       // Delegate to store the function pointer
    );

    if (processEventStatus != 0)
    {
        LoggerSingleton::Instance().LogError("Failed to bind ProcessEvent delegate.");
        return false;
    }

    return createInstanceDelegateStatus == 0 &&
        releaseInstanceDelegateStatus == 0 &&
        updateDelegateStatus == 0 &&
        getScriptInfoStatus == 0 &&
        assemblyInitStatus == 0 &&
        assemblyUnloadStatus == 0 && 
        releaseScriptInfoStatus == 0 && 
        processEventStatus == 0;
}

// Helper functions to load CoreCLR
HMODULE ScriptEngine::LoadLibrary(const char* path)
{
    return LoadLibraryA(path);
}

FARPROC ScriptEngine::GetFunction(HMODULE library, const char* name)
{
    return GetProcAddress(library, name);
}

std::string ScriptEngine::GetExecutablePath(bool removeFileName)
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    std::string path(buffer);

    if (removeFileName)
    {
        size_t lastSlash = path.find_last_of("\\/");
        if (lastSlash != std::string::npos)
        {
            path = path.substr(0, lastSlash);
        }
    }

    return path;
}
