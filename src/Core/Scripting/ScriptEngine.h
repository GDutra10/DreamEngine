#ifndef CORE_SCRIPTING_SCRIPT_ENGINE_H_
#define CORE_SCRIPTING_SCRIPT_ENGINE_H_

#include <string>
#include <vector>
//#include <windows.h>
#include "coreclrhost.h"
#include "ScriptInfo.h"

namespace DreamEngine::Core::Scripting
{

// to avoid add windows.h
#define MAX_PATH 260

typedef void* HMODULE;
typedef void* FARPROC;
typedef unsigned long DWORD;

extern "C"
{
    __declspec(dllimport) HMODULE __stdcall LoadLibraryA(const char* lpLibFileName);
    __declspec(dllimport) FARPROC __stdcall GetProcAddress(HMODULE hModule, const char* lpProcName);
    __declspec(dllimport) int __stdcall FreeLibrary(HMODULE hModule);
    __declspec(dllimport) DWORD __stdcall GetModuleFileNameA(HMODULE hModule, char* lpFilename, DWORD nSize);
}

// scripts manager
typedef void* (*createInstanceDelegate)(const char*, const char*);
typedef void (*releaseInstanceDelegate)(void*);
typedef void (*updateDelegate)(void*, void*);
typedef void* (*getScriptInfoDelegate)(int*);
typedef void (*releaseScriptInfoDelegate)(void*, int*);
typedef void (*updateGameDelegate)(void*);

// assembly manager
typedef void (*assemblyInitializeDelegate)(const char*);
typedef void (*assembliesUnloadDelegate)();

class ScriptEngine
{
   public:
    ScriptEngine();
    ~ScriptEngine();

    void LoadAssembly(const std::string& assemblyPath);
    void UnloadAssembly();
    void Shutdown();
    bool IsRunning();

    static void* CreateInstance(const char* assemblyName, const char* typeName);
    static void ReleaseInstance(void* instance);
    static void Update(void* instance, void* entityData);
    static void UpdateGame(void* gameData);
    std::vector<ScriptInfo> GetClassInfoList();

   private:
    // .NET runtime
    static void* m_spHostHandle;
    static unsigned int m_sDomainId;
    void* m_pCoreClrLibrary = nullptr;
    bool m_isAssemblyLoaded = false;
    std::string m_coreClrPath;
    // coreclr functions
    static coreclr_initialize_ptr m_spCoreClrInitialize;
    static coreclr_create_delegate_ptr m_spCoreClrCreateDelegate;
    static coreclr_shutdown_ptr m_spCoreClrShutdown;
    // scripts delegate
    static updateDelegate m_spUpdateDelegate;
    static createInstanceDelegate m_spCreateInstanceDelegate;
    static releaseInstanceDelegate m_spReleaseInstanceDelegate;
    static getScriptInfoDelegate m_spGetScriptInfoDelegate;
    static releaseScriptInfoDelegate m_spReleaseScriptInfoDelegate;
    static updateGameDelegate m_spUpdateGameDelegate;
    // assembly delegate
    static assemblyInitializeDelegate m_spAssemblyInitializeDelegate;
    static assembliesUnloadDelegate m_spAssembliesUnloadDelegate;

    bool Initialize();
    static std::string FindCoreCLRPath();
    bool LoadCoreCLR(const std::string& coreClrPath);
    bool InitCoreCLR();
    bool CreateCoreCLRDelegates();
    static std::string NormalizePath(const std::string& path);

    // helper functions
    static HMODULE LoadLibrary(const char* path);
    static std::string BuildTpaList(const std::string& coreClrPath);
    static FARPROC GetFunction(HMODULE library, const char* name);
    static std::string GetExecutablePath(bool removeFileName = false);
};
}  // namespace DreamEngine::Core::Scripts
#endif