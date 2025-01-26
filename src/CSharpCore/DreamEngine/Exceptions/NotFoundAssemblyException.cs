namespace DreamEngine.Exceptions;

public class NotFoundAssemblyException : Exception
{
    public NotFoundAssemblyException(string assemblyName) : base($"Asembly '{assemblyName}' not found!")
    {
    }
}