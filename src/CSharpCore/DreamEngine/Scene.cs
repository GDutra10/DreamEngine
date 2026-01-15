using DreamEngine.ECS;

namespace DreamEngine;

public class Scene
{
    public Dictionary<uint, Entity> Entities { get; } = [];
}