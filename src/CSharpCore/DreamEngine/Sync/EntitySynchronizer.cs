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

        // ### Components to EntityData ###

        // ui component
        entityData.uiComponentHas = HasComponentInByte(entity.GetComponent<UiComponent>());

        // camera component
        var cameraComponent = entity.GetComponent<CameraComponent>();
        entityData.cameraComponentHas = HasComponentInByte(cameraComponent);
        
        if (cameraComponent is not null)
        {
            entityData.cameraFar = cameraComponent.Far;
            entityData.cameraNear = cameraComponent.Near;
            entityData.cameraFov = cameraComponent.Fov;
        }

        // directional light component
        var directionalLightComponent = entity.GetComponent<DirectionalLightComponent>();
        entityData.directionalLightComponentHas = HasComponentInByte(directionalLightComponent);

        if (directionalLightComponent is not null)
        {
            entityData.directionalLightColorRed = directionalLightComponent.Color.Red;
            entityData.directionalLightColorGreen = directionalLightComponent.Color.Green;
            entityData.directionalLightColorBlue = directionalLightComponent.Color.Blue;
            entityData.directionalLightColorAlpha = directionalLightComponent.Color.Alpha;

            entityData.directionalLightSpecularX = directionalLightComponent.Specular.X;
            entityData.directionalLightSpecularY = directionalLightComponent.Specular.Y;
            entityData.directionalLightSpecularZ = directionalLightComponent.Specular.Z;

            entityData.directionalLightInfluence = directionalLightComponent.Influence;
        }
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

        HandleComponentToEntity<UiComponent>(ref entityData, entity, entityData.uiComponentHas);
        HandleComponentToEntity<CameraComponent>(ref entityData, entity, entityData.cameraComponentHas, HandleCreateCameraComponent);
        HandleComponentToEntity<DirectionalLightComponent>(ref entityData, entity, entityData.directionalLightComponentHas, HandleCreateDirectionalLightComponent);
    }

    private static byte HasComponentInByte(Component? component)
    {
        return (byte)(component is not null ? 1 : 0);
    }

    private static void HandleComponentToEntity<T>(ref EntityData entityData, Entity entity, byte hasComponent, Action<EntityData, T>? action = null) 
        where T : Component
    {
        var component = entity.GetComponent<T>();

        if (hasComponent == 0)
        {
            if (component is null)
                return;

            entity.RemoveComponent(component);

            return;
        }

        component ??= entity.AddComponent<T>();
        action?.Invoke(entityData, component);
    }

    private static void HandleCreateCameraComponent(EntityData entityData, CameraComponent cameraComponent)
    {
        cameraComponent.Far = entityData.cameraFar;
        cameraComponent.Fov = entityData.cameraFov;
        cameraComponent.Near = entityData.cameraNear;
    }

    private static void HandleCreateDirectionalLightComponent(EntityData entityData, DirectionalLightComponent directionalLightComponent)
    {
        directionalLightComponent.Color.Red = entityData.directionalLightColorRed;
        directionalLightComponent.Color.Green = entityData.directionalLightColorGreen;
        directionalLightComponent.Color.Blue = entityData.directionalLightColorBlue;
        directionalLightComponent.Color.Alpha = entityData.directionalLightColorAlpha;

        directionalLightComponent.Specular.X = entityData.directionalLightSpecularX;
        directionalLightComponent.Specular.Y = entityData.directionalLightSpecularY;
        directionalLightComponent.Specular.Z = entityData.directionalLightSpecularZ;

        directionalLightComponent.Influence = entityData.directionalLightInfluence;
    }
}