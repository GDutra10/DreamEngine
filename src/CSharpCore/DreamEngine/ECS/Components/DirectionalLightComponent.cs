using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace DreamEngine.ECS.Components;
public class DirectionalLightComponent : Component
{
    public Vector3 Specular = Vector3.Zero;
    public Color Color = new ();
    public float Influence = 1.0f;

    internal DirectionalLightComponent(uint entityId) : base(entityId)
    {
    }
}
