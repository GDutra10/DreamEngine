using DreamEngine.ECS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DreamEngine.Commands;
internal class ParentCommand : Command
{
    private readonly Entity? _parent;

    public ParentCommand(Entity entity, Entity? parent) : base(entity) 
    {
        _parent = parent;
    }

    public override void Process()
    {
        Entity.InternalParent = _parent;
    }

    public override string ToString()
    {
        return $"ParentCommand: Entity {Entity.Id}, Parent {_parent?.Id.ToString() ?? "null"}";
    }
}
