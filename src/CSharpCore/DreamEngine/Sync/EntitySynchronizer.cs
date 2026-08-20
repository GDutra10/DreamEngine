using System.Globalization;
using System.Reflection;
using DreamEngine.Core;
using DreamEngine.ECS;
using DreamEngine.ECS.Components;
using DreamEngine.Extensions;
using DreamEngine.Sync.Data;

namespace DreamEngine.Sync;

internal static class EntitySynchronizer
{
    // must use ref here, because we want to modify the entityDTO
    // without ref, c# pass it a copy of that struct
    public static void SynchronizeFromTo(Entity entity, ref EntityData entityData)
    {
        entityData.active = entity.Active.ToByte();
        entityData.parentId = entity.Parent?.Id ?? 1;
        entityData.parentHas = (byte)(entity.Parent is not null ? 1 : 0);

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

        // collider component
        var colliderComponent = entity.GetComponent<ColliderComponent>();
        entityData.colliderComponentHas = HasComponentInByte(colliderComponent);

        if (colliderComponent is not null)
        {
            entityData.colliderEnabled = colliderComponent.Enabled.ToByte();
            entityData.colliderIsTrigger = colliderComponent.IsTrigger.ToByte();

            entityData.colliderCenterX = colliderComponent.Center.X;
            entityData.colliderCenterY = colliderComponent.Center.Y;
            entityData.colliderCenterZ = colliderComponent.Center.Z;

            entityData.colliderSizeX = colliderComponent.Size.X;
            entityData.colliderSizeY = colliderComponent.Size.Y;
            entityData.colliderSizeZ = colliderComponent.Size.Z;
        }

        // audio listener component
        var audioListenerComponent = entity.GetComponent<AudioListenerComponent>();
        entityData.audioListenerComponentHas = HasComponentInByte(audioListenerComponent);

        if (audioListenerComponent is not null)
            entityData.audioListenerEnabled = audioListenerComponent.Enabled.ToByte();

        // audio emitter component
        var audioEmitterComponent = entity.GetComponent<AudioEmitterComponent>();
        entityData.audioEmitterComponentHas = HasComponentInByte(audioEmitterComponent);

        if (audioEmitterComponent is not null)
        {
            entityData.audioEmitterEnabled = audioEmitterComponent.Enabled.ToByte();
            entityData.audioEmitterSpatial = audioEmitterComponent.Spatial.ToByte();

            entityData.audioEmitterMaxDistance = audioEmitterComponent.MaxDistance;
            entityData.audioEmitterMinDistance = audioEmitterComponent.MinDistance;
            entityData.audioEmitterPitch = audioEmitterComponent.Pitch;
            entityData.audioEmitterVolume = audioEmitterComponent.Volume;
        }
    }

    // using the ref keyword is the same case as above
    public static void SynchronizeFromTo(ref EntityData entityData, Entity entity)
    {
        entity.Id = entityData.id;
        entity.Active = entityData.active == 1;

        if (entityData.parentHas == 1)
            entity.Parent = Game.Scene.GetEntity(entityData.parentId);

        entity.Transform.Position.InternalX = entityData.transformPositionX;
        entity.Transform.Position.InternalY = entityData.transformPositionY;
        entity.Transform.Position.InternalZ = entityData.transformPositionZ;
        entity.Transform.Rotation.InternalX = entityData.transformRotationX;
        entity.Transform.Rotation.InternalY = entityData.transformRotationY;
        entity.Transform.Rotation.InternalZ = entityData.transformRotationZ;
        entity.Transform.Scale.InternalX = entityData.transformScaleX;
        entity.Transform.Scale.InternalY = entityData.transformScaleY;
        entity.Transform.Scale.InternalZ = entityData.transformScaleZ;

        HandleComponentToEntity<UiComponent>(ref entityData, entity, entityData.uiComponentHas);
        HandleComponentToEntity<CameraComponent>(ref entityData, entity, entityData.cameraComponentHas, HandleCreateCameraComponent);
        HandleComponentToEntity<DirectionalLightComponent>(ref entityData, entity, entityData.directionalLightComponentHas, HandleCreateDirectionalLightComponent);
        HandleComponentToEntity<ColliderComponent>(ref entityData, entity, entityData.colliderComponentHas, HandleCreateColliderComponent);
        HandleComponentToEntity<AudioListenerComponent>(ref entityData, entity, entityData.audioListenerComponentHas, HandleCreateAudioListenerComponent);
        HandleComponentToEntity<AudioEmitterComponent>(ref entityData, entity, entityData.audioEmitterComponentHas, HandleCreateAudioEmitterComponent);
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

    private static void HandleCreateColliderComponent(EntityData entityData, ColliderComponent colliderComponent)
    {
        colliderComponent.Enabled = entityData.colliderEnabled == 1;
        colliderComponent.IsTrigger = entityData.colliderIsTrigger == 1;

        colliderComponent.Center.X = entityData.colliderCenterX;
        colliderComponent.Center.Y = entityData.colliderCenterY;
        colliderComponent.Center.Z = entityData.colliderCenterZ;

        colliderComponent.Size.X = entityData.colliderSizeX;
        colliderComponent.Size.Y = entityData.colliderSizeY;
        colliderComponent.Size.Z = entityData.colliderSizeZ;
    }

    private static void HandleCreateAudioListenerComponent(EntityData data, AudioListenerComponent component)
    {
        component.Enabled = data.audioListenerEnabled == 1;
    }

    private static void HandleCreateAudioEmitterComponent(EntityData data, AudioEmitterComponent component)
    {
        component.Enabled = data.audioEmitterEnabled == 1;
        component.Spatial = data.audioEmitterSpatial == 1;

        component.MaxDistance = data.audioEmitterMaxDistance;
        component.MinDistance = data.audioEmitterMinDistance;
        component.Pitch = data.audioEmitterPitch;
        component.Volume = data.audioEmitterVolume;
    }
}