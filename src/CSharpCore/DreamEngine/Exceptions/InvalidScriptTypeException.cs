using System;

namespace DreamEngine.Exceptions;

public class InvalidScriptTypeException : Exception
{
    public InvalidScriptTypeException(Type type) : base($"Type '{type.FullName}' have not inheritance with Script!")
    {
    }
}