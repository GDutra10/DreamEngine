using DreamEngine.ECS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DreamEngine.Commands;
internal abstract class Command
{
    public Entity Entity { get; private set; }

    public Command(Entity entity)
    {
        Entity = entity;
    }

    public abstract void Process();
}
