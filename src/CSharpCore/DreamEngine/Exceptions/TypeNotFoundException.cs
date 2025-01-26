using System;

namespace DreamEngine.Exceptions;

public class TypeNotFoundException : Exception
{
    public TypeNotFoundException(string typeName, string assemblyName) : base($"Type '{typeName}' not found in '{assemblyName}' assembly!")
    {
    }
}