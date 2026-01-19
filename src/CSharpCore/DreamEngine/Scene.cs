using DreamEngine.Core;
using DreamEngine.ECS;
using DreamEngine.ECS.Components;

namespace DreamEngine;

public class Scene
{
    public bool ShowCursor { get; internal set; }
    
    public Transform CameraTransform => CameraEntity.Transform;

    public SceneEnvironment Environment => environment;
    
    internal SceneEnvironment environment = new ();

    internal Dictionary<uint, Entity> Entities { get; } = [];

    internal Entity CameraEntity { get; set; } = null!;

    public Entity CreateEntity(string tag, string name = "")
    {
        var entity = SceneManager.CreateEntity(tag, name);

        Entities[entity.Id] = entity;

        return entity;
    }

    public void SetShowCursor(bool showCursor)
    {
        if (!SceneManager.SetShowCursor(showCursor))
            return;

        ShowCursor = showCursor;
    }

    public void SetGlobalLight(Color color, float intensity)
    {
        if (!SceneManager.SetGlobalLight(color.Red, color.Green, color.Blue, intensity))
            return;

        environment.Light = new Color()
        {
            Red = color.Red,
            Green = color.Green,
            Blue = color.Blue,
        };

        environment.LightIntensity = intensity;
    }

    public void SetCamera(CameraComponent cameraComponent)
    {
        var entityId = cameraComponent.EntityId;

        if (!SceneManager.SetCamera(entityId))
            return;

        CameraEntity = Entities[entityId];
    }

    public void ChangeScene(string sceneName)
    {
        if (!SceneManager.ChangeScene(sceneName))
            return;

        Entities.Clear();
    }

    internal Entity CreateEntity(uint id)
    {
        var entity = new Entity()
        {
            Id = id,
        };

        Entities[id] = entity;

        return entity;
    }

    public struct SceneEnvironment
    {
        public Color Light { get; set; }
        public float LightIntensity { get; set; }
    }
}