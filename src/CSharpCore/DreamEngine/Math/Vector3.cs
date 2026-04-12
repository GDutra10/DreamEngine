using DreamEngine.Commands;
using DreamEngine.ECS;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace DreamEngine.Math;
public class Vector3 : IEquatable<Vector3>
{
    public float X 
    {
        get => InternalX;
        set
        {
            if (_owner?.MustProcessCommandsNextUpdate ?? false)
                CommandQueue.Add(new Vector3Command(_owner, this, value, InternalY, InternalZ));

            InternalX = value;
        }
    }
    public float Y 
    {
        get => InternalY;
        set
        {
            if (_owner?.MustProcessCommandsNextUpdate ?? false)
                CommandQueue.Add(new Vector3Command(_owner, this, InternalX, value, InternalZ));

            InternalY = value;
        }
    }
    public float Z
    {
        get => InternalZ;
        set
        {
            if (_owner?.MustProcessCommandsNextUpdate ?? false)
                CommandQueue.Add(new Vector3Command(_owner, this, InternalX, InternalY, value));

            InternalZ = value;
        }
    }

    private readonly Entity? _owner;
    internal float InternalX;
    internal float InternalY;
    internal float InternalZ;

    public Vector3() { }

    internal Vector3(Entity owner)
    {
        this._owner = owner;
    }

    public static Vector3 Zero
    {
        get
        {
            return new Vector3
            {
                X = 0,
                Y = 0,
                Z = 0
            };
        }
    }

    public static Vector3 operator+(Vector3 left, Vector3 right)
    {
        return new Vector3
        {
            X = left.X + right.X,
            Y = left.Y + right.Y,
            Z = left.Z + right.Z
        };
    }

    public static Vector3 operator -(Vector3 left, Vector3 right)
    {
        return new Vector3
        {
            X = left.X - right.X,
            Y = left.Y - right.Y,
            Z = left.Z - right.Z
        };
    }

    public static bool operator ==(Vector3 left, Vector3 right)
    {
        return ReferenceEquals(left, right) || (left is not null && right is not null && left.Equals(right));
    }

    public static bool operator !=(Vector3 left, Vector3 right)
    {
        return !(ReferenceEquals(left, right) && left is not null && right is not null && left.Equals(right));
    }

    public bool Equals(Vector3? other)
    {
        return other is not null && X == other.X && Y == other.Y && Z == other.Y;
    }

    public override bool Equals(object obj)
    {
        return Equals(obj as Vector3);
    }

    public override int GetHashCode()
    {
        return X.GetHashCode() ^ Y.GetHashCode() ^ Z.GetHashCode();
    }
}
