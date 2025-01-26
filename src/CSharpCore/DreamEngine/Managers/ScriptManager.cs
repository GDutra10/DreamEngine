using DreamEngine.Core;
using DreamEngine.Exceptions;
using DreamEngine.Scripting;
using DreamEngine.Sync;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;

namespace DreamEngine.Managers;

internal static class ScriptManager
{
    private static AssemblyLoadContext? _scriptLoadContext = null;
    private static Assembly? _assembly = null;

    public static void AssemblyInitialize(string assemblyPath)
    {
        Logger.LogInfo($"Initializing Assembly: {assemblyPath}");

        try
        {
            _scriptLoadContext = new AssemblyLoadContext(assemblyPath, true);

            // this cannot allow to unload assembly because it's loaded in the default context
            //_assembly = System.Runtime.Loader.AssemblyLoadContext.Default.LoadFromAssemblyPath(assemblyPath);

            // this cause some problems with the unload, because the core clr keep the file locked
            //_assembly = _scriptLoadContext.LoadFromAssemblyPath(assemblyPath);

            // Load the assembly into a byte array
            byte[] assemblyBytes = File.ReadAllBytes(assemblyPath);

            using var stream = new MemoryStream(assemblyBytes);
            _assembly = _scriptLoadContext.LoadFromStream(stream);
        }
        catch (Exception e)
        {
            Logger.LogError(e.Message);
        }
    }

    public static void AssembliesUnload()
    {
        // TODO: validations

        Logger.LogInfo($"Unload Assembly: {_assembly.FullName}");

        try
        {
            _scriptLoadContext!.Unload();
            _scriptLoadContext = null;
            _assembly = null;

            GC.Collect();
            GC.WaitForPendingFinalizers();

            Logger.LogInfo("Assembly has been unloaded!");
        }
        catch (Exception e)
        {
            Logger.LogError(e.ToString());
        }
    }

    public static IntPtr CreateInstance(string assemblyName, string name)
    {
        try
        {
            if (_assembly is null || _assembly.GetName().Name != assemblyName)
                throw new NotFoundAssemblyException(assemblyName);

            var type = _assembly.GetType(name);

            if (type is null)
                throw new TypeNotFoundException(name, assemblyName);

            var instance = Activator.CreateInstance(type);

            if (!type.IsSubclassOf(typeof(Script)))
                throw new InvalidScriptTypeException(type);

            // create on GC
            var handle = GCHandle.Alloc(instance);

            return GCHandle.ToIntPtr(handle);
        } 
        catch (Exception e)
        {
            Logger.LogError(e.ToString());
        }
        
        return IntPtr.Zero;
    }

    public static void ReleaseInstance(IntPtr handle)
    {
        GCHandle.FromIntPtr(handle).Free();
    }

    public static void UpdateInstance(IntPtr handle, IntPtr entityDataHandle)
    {
        if (GCHandle.FromIntPtr(handle).Target is not Script script)
        {
            Logger.LogError("Pointer is not a Script");
            
            return;
        }

        try
        {
            var entityData = Marshal.PtrToStructure<EntityData>(entityDataHandle);
            
            EntitySynchronizer.SynchronizeFromTo(ref entityData, script.Entity);
            script.Initialize();
            script.Update();
            EntitySynchronizer.SynchronizeFromTo(script.Entity, ref entityData);

            // Marshal the updated structure back to unmanaged memory
            Marshal.StructureToPtr(entityData, entityDataHandle, false);
        }
        catch (Exception e)
        {
            Logger.LogError(e.ToString());
        }
    }

    public static IntPtr GetScriptInfo(out int count)
    {
        var result = new List<ScriptInfo>();

        foreach (var assembly in _scriptLoadContext.Assemblies)
        {
            foreach (var type in assembly.GetTypes())
            {
                if (type.IsSubclassOf(typeof(Script)))
                {
                    Logger.LogInfo($"Script Found: {type.FullName}, {assembly.GetName().Name}");

                    result.Add(new ScriptInfo
                    {
                        AssemblyName = Marshal.StringToHGlobalAnsi(assembly.GetName().Name),
                        Name = Marshal.StringToHGlobalAnsi(type.FullName)
                    });
                }
            }
        }

        count = result.Count;

        // Allocate memory for the result array
        var size = Marshal.SizeOf<ScriptInfo>();
        var ptr = Marshal.AllocHGlobal(size * count);

        for (int i = 0; i < count; i++)
        {
            var structPtr = IntPtr.Add(ptr, i * size);
            Marshal.StructureToPtr(result[i], structPtr, false);
        }

        return ptr;
    }

    public static void ReleaseScriptInfos(IntPtr ptr, int count)
    {
        var size = Marshal.SizeOf(typeof(ScriptInfo));

        for (var i = 0; i < count; i++)
        {
            IntPtr structPtr = IntPtr.Add(ptr, i * size);
            var native = Marshal.PtrToStructure<ScriptInfo>(structPtr);

            Marshal.FreeHGlobal(native.AssemblyName);
            Marshal.FreeHGlobal(native.Name);
        }

        Marshal.FreeHGlobal(ptr);
    }

    public static void UpdateGame(IntPtr gameDataHandle)
    {
        try
        {
            var gameData = Marshal.PtrToStructure<GameData>(gameDataHandle);
            GameSynchronizer.Synchronize(ref gameData);
        }
        catch (Exception e)
        {
            Logger.LogError(e.ToString());
        }
    }
}
