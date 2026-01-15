using DreamEngine.ECS;
using DreamEngine.Managers;
using DreamEngine.Scripting;
using System.Runtime.InteropServices;
using DreamEngine.Core;

namespace DreamEngine.Sync;
internal static class GameSynchronizer
{
    public static unsafe void Synchronize(ref GameData gameData, EntityData* entities, int entityCount)
    {
        Game.DeltaTime = gameData.deltaTime;
        InputSynchronizer.Synchronize(ref gameData.inputData);

        if (gameData.mustRecreateEntities == 1)
            Game.Scene.Entities.Clear();

        var aliveIds = new HashSet<uint>();

        for (var i = 0; i < entityCount; i++)
        {
            ref var data = ref entities[i];
            aliveIds.Add(data.id);

            if (!Game.Scene.Entities.TryGetValue(data.id, out var entity))
            {
                entity = new Entity();
                entity.SetEntityData(ref data);
                Game.Scene.Entities[data.id] = entity;
            }

            EntitySynchronizer.SynchronizeFromTo(ref data, entity);

            if (entity.Script == null)
            {
                var assembly = data.GetAssemblyName();
                var className = data.GetClassName();

                if (!string.IsNullOrWhiteSpace(assembly) &&
                    !string.IsNullOrWhiteSpace(className))
                {
                    TryCreateScript(entity, assembly, className);
                }
            }
        }

        // Cleanup removed entities
        var toRemove = Game.Scene.Entities.Keys
            .Where(id => !aliveIds.Contains(id))
            .ToList();

        foreach (var id in toRemove)
            Game.Scene.Entities.Remove(id);
    }

    private static void TryCreateScript(Entity entity, string asm, string cls)
    {
        try
        {
            var ptr = ScriptManager.CreateInstance(asm, cls);
            
            if (ptr == IntPtr.Zero)
                return;

            entity.Script = (Script)GCHandle.FromIntPtr(ptr).Target!;
            entity.Script.Initialize(entity);
        }
        catch (Exception e)
        {
            Logger.LogError(e.ToString());
        }
    }
}
