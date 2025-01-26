using System;
using System.Numerics;

namespace DreamEngine.Inputs;

public static class Input
{
    internal static Vector2 MousePosition { get; private set; } = Vector2.Zero;
    internal static readonly Dictionary<Key, KeyState> KeyStates = new();
    internal static readonly Dictionary<MouseButton, KeyState> MouseButtonStates = new();
    internal static bool WasInitialized = false;

    public static Vector2 GetMousePosition()
    {
        return Vector2.Zero;
    }

    public static bool IsPressed(Key key)
    {
        return KeyStates[key] == KeyState.Press;
    }

    public static bool IsPressed(MouseButton mouseButton)
    {
        return MouseButtonStates[mouseButton] == KeyState.Press;
    }

    internal static void InitializeInput()
    {
        if (!WasInitialized)
            return;

        foreach (var key in Enum.GetValues(typeof(Key)))
            KeyStates.Add((Key)key, KeyState.Release);

        foreach (var mouseButton in Enum.GetValues(typeof(MouseButton)))
            MouseButtonStates.Add((MouseButton)mouseButton, KeyState.Release);

        WasInitialized = true;
    }

    internal static void SetKeyState(Key key, KeyState keyState)
    {
        KeyStates[key] = keyState;
    }

    internal static void SetMouseButtonState(MouseButton mouseButton, KeyState keyState)
    {
        MouseButtonStates[mouseButton] = keyState;
    }

    internal static void SetMousePosition(float x, float y)
    {
        MousePosition = new Vector2(x, y);
    }
}