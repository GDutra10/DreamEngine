using DreamEngine.Core;
using DreamEngine.ECS;
using DreamEngine.ECS.Components;

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

        HandleUiComponent(ref entityData, entity);
    }

    private static void HandleUiComponent(ref EntityData entityData, Entity entity)
    {
        var uiComponent = entity.GetComponent<UiComponent>();

        if (!entityData.uiComponentHas)
        {
            if (uiComponent is null)
                return;

            entity.RemoveComponent(uiComponent);
            
            return;
        }

        if (uiComponent is null)
            entity.AddComponent<UiComponent>(new UiComponent(entity.Id));
    }
}