using System.Globalization;
using System.Reflection;
using DreamEngine.Core;
using DreamEngine.ECS;
using DreamEngine.ECS.Components;
using DreamEngine.Sync.Data;

namespace DreamEngine.Sync;

internal static class EntitySynchronizer
{
    // must use ref here, because we want to modify the entityDTO
    // without ref, c# pass it a copy of that struct
    public static void SynchronizeFromTo(Entity entity, ref EntityData entityData)
    {
        entityData.transformPositionX = entity.Transform.Position.X;
        entityData.transformPositionY = entity.Transform.Position.Y;
        entityData.transformPositionZ = entity.Transform.Position.Z;
        entityData.transformRotationX = entity.Transform.Rotation.X;
        entityData.transformRotationY = entity.Transform.Rotation.Y;
        entityData.transformRotationZ = entity.Transform.Rotation.Z;
        entityData.transformScaleX = entity.Transform.Scale.X;
        entityData.transformScaleY = entity.Transform.Scale.Y;
        entityData.transformScaleZ = entity.Transform.Scale.Z;
    }

    // using the ref keyword is the same case as above
    public static void SynchronizeFromTo(ref EntityData entityData, Entity entity)
    {
        entity.Id = entityData.id;
        entity.Transform.Position.X = entityData.transformPositionX;
        entity.Transform.Position.Y = entityData.transformPositionY;
        entity.Transform.Position.Z = entityData.transformPositionZ;
        entity.Transform.Rotation.X = entityData.transformRotationX;
        entity.Transform.Rotation.Y = entityData.transformRotationY;
        entity.Transform.Rotation.Z = entityData.transformRotationZ;
        entity.Transform.Scale.X = entityData.transformScaleX;
        entity.Transform.Scale.Y = entityData.transformScaleY;
        entity.Transform.Scale.Z = entityData.transformScaleZ;

        HandleComponent<UiComponent>(ref entityData, entity, entityData.uiComponentHas);
        HandleComponent<CameraComponent>(ref entityData, entity, entityData.uiComponentHas, HandleCreateCameraComponent);
    }

    private static void HandleComponent<T>(ref EntityData entityData, Entity entity, byte hasComponent, Action<EntityData, T>? action = null) 
        where T : Component
    {
        var component = entity.GetComponent<T>();

        if (hasComponent == 1)
        {
            if (component is null)
                return;

            entity.RemoveComponent(component);

            return;
        }

        if (component is null)
        {
            component = (T)Activator.CreateInstance(
                type: typeof(T), 
                bindingAttr: BindingFlags.Instance | BindingFlags.NonPublic,
                binder: null,
                args: [ entity.Id ],
                culture: CultureInfo.CurrentCulture)!;
            entity.AddComponent(component);
        }

        action?.Invoke(entityData, component);
    }

    private static void HandleCreateCameraComponent(EntityData entityData, CameraComponent cameraComponent)
    {
        cameraComponent.Far = entityData.cameraFar;
        cameraComponent.Fov = entityData.cameraFov;
        cameraComponent.Near = entityData.cameraNear;
    }
}