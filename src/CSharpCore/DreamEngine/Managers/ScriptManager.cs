using DreamEngine.Core;
using DreamEngine.Exceptions;
using DreamEngine.Scripting;
using DreamEngine.Sync;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.Loader;
using DreamEngine.ECS;

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

    public static unsafe void UpdateGame(IntPtr gameDataHandle, IntPtr entitiesDataArrayPtr, int entityCount)
    {
        try
        {
            var gameData = Marshal.PtrToStructure<GameData>(gameDataHandle);
            var entityData = (EntityData*)entitiesDataArrayPtr;

            // Sync managed scene from native data
            GameSynchronizer.Synchronize(ref gameData, entityData, entityCount);

            // Scripts
            foreach (var entity in Game.Scene.Entities)
                entity.Value.Script?.Update();

            // Map id -> index once
            var indexById = new Dictionary<uint, int>(entityCount);
            
            for (var i = 0; i < entityCount; i++)
                indexById[entityData[i].id] = i;

            // Write changes directly into native buffer
            foreach (var kv in Game.Scene.Entities)
            {
                if (!indexById.TryGetValue(kv.Key, out var idx))
                    continue;

                EntitySynchronizer.SynchronizeFromTo(kv.Value, ref entityData[idx]);
            }

            // Write back game data (only if you actually modify it)
            Marshal.StructureToPtr(gameData, gameDataHandle, false);
        }
        catch (Exception e)
        {
            Logger.LogError(e.ToString());
        }
    }

    public static void ProcessEvent(int eventId)
    {
        try
        {
            UiManager.HandleEvent(eventId);
        }
        catch (Exception e)
        {
            Logger.LogError(e.ToString());
        }
    }

    internal static IntPtr CreateInstance(string assemblyName, string name)
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
}
