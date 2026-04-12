using DreamEngine.Core;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DreamEngine.Commands;
internal static class CommandQueue
{
    private static List<Command> _commands = [];

    public static void Add(Command command)
    {
        _commands.Add(command);
    }

    public static void Process()
    {
        foreach (var command in _commands)
        {
            Logger.LogTrace(command.ToString() ?? "Command To String is null");
            command.Process();
            command.Entity.MustProcessCommandsNextUpdate = false;
        }

        _commands.Clear();
    }
}
