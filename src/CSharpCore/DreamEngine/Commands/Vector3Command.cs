using DreamEngine.ECS;
using DreamEngine.Math;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DreamEngine.Commands;
internal class Vector3Command : Command
{
    private float _x;
    private float _y; 
    private float _z;
    public Vector3 _vector;

    public Vector3Command(Entity entity, Vector3 vector, float x, float y, float z) : base(entity)
    {
        _vector = vector;
        _x = x;
        _y = y;
        _z = z;
    }

    public override void Process()
    {
        _vector.InternalX = _x;
        _vector.InternalY = _y;
        _vector.InternalZ = _z;
    }

    public override string ToString()
    {
        return $"Vector3Command: Entity {Entity.Id}, X: {_x}, Y: {_y}, Z: {_z}";
    }
}
