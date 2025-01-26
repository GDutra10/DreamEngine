using DreamEngine.Core;
using DreamEngine.ECS;

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
    public static void SynchronizeFromTo(ref EntityData EntityData, Entity entity)
    {
        entity.Transform.Position.X = EntityData.transformPositionX;
        entity.Transform.Position.Y = EntityData.transformPositionY;
        entity.Transform.Position.Z = EntityData.transformPositionZ;
        entity.Transform.Rotation.X = EntityData.transformRotationX;
        entity.Transform.Rotation.Y = EntityData.transformRotationY;
        entity.Transform.Rotation.Z = EntityData.transformRotationZ;
        entity.Transform.Scale.X = EntityData.transformScaleX;
        entity.Transform.Scale.Y = EntityData.transformScaleY;
        entity.Transform.Scale.Z = EntityData.transformScaleZ;
    }
}