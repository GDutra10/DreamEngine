using DreamEngine.Core;
using DreamEngine.ECS;
using DreamEngine.Managers;
using DreamEngine.Scripting;
using DreamEngine.Sync.Data;
using System.Runtime.InteropServices;

namespace DreamEngine.Sync;
internal static class SceneSynchronizer
{
    public static void SynchronizeFromTo(Scene scene, ref SceneData sceneData)
    {
        sceneData.showCursor = (byte) (scene.ShowCursor ? 1 : 0);
        sceneData.globalLightColorR = scene.Environment.Light.Red;
        sceneData.globalLightColorG = scene.Environment.Light.Green;
        sceneData.globalLightColorB = scene.Environment.Light.Blue;
        sceneData.globalLightIntensity = scene.Environment.LightIntensity;
        sceneData.mainCameraEntityId = scene.CameraEntity.Id;
    }

    public static unsafe void SynchronizeFromTo(ref SceneData sceneData, Scene scene, EntityData* entities, int entityCount)
    {
        scene.ShowCursor = sceneData.showCursor == 1;
        scene.environment.Light = new Color(
            sceneData.globalLightColorR, 
            sceneData.globalLightColorG, 
            sceneData.globalLightColorB);
        scene.environment.LightIntensity = sceneData.globalLightIntensity;

        if (sceneData.mustRecreateEntities == 1)
            scene.Entities.Clear();

        var aliveIds = new HashSet<uint>();

        for (var i = 0; i < entityCount; i++)
        {
            ref var data = ref entities[i];
            aliveIds.Add(data.id);

            if (!scene.Entities.TryGetValue(data.id, out var entity))
            {
                entity = scene.CreateEntity(data.id);
                entity.SetEntityData(ref data);
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
        var toRemove = scene.Entities.Keys
            .Where(id => !aliveIds.Contains(id))
            .ToList();

        foreach (var id in toRemove)
            scene.Entities.Remove(id);

        if (sceneData.mainCameraEntityId != 0)
            scene.CameraEntity = scene.Entities[sceneData.mainCameraEntityId];
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
