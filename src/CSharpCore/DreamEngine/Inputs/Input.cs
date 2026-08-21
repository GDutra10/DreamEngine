using DreamEngine.Extensions;
using DreamEngine.Sync.Data;
using System;
using System.Numerics;

namespace DreamEngine.Inputs;

public static class Input
{
    internal static Vector2 MousePosition { get; private set; } = Vector2.Zero;
    internal static readonly Dictionary<Key, ButtonState> KeyStates = new();
    internal static readonly Dictionary<MouseButton, ButtonState> MouseButtonStates = new();
    internal static bool WasInitialized = false;

    internal class ButtonState
    {
        public bool Current = false;
        public bool Previous = false;
    }

    public static Vector2 GetMousePosition()
    {
        return Vector2.Zero;
    }

    /// <summary>
    /// Every frame while held
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    public static bool IsDown(Key key)
    {
        return KeyStates[key].Current;
    }

    /// <summary>
    /// First frame the key goes down
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    public static bool IsPressed(Key key)
    {
        var state = KeyStates[key];

        return state.Current && !state.Previous;
    }

    /// <summary>
    /// First frame the key goes up
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    public static bool IsUp(Key key)
    {
        var state = KeyStates[key];

        return !state.Current && state.Previous;
    }

    /// <summary>
    /// Every frame while held
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    public static bool IsDown(MouseButton mouseButton)
    {
        return MouseButtonStates[mouseButton].Current;
    }

    /// <summary>
    /// First frame the key goes down
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    public static bool IsPressed(MouseButton mouseButton)
    {
        var state = MouseButtonStates[mouseButton];

        return state.Current && !state.Previous;
    }

    /// <summary>
    /// First frame the key goes up
    /// </summary>
    /// <param name="key"></param>
    /// <returns></returns>
    public static bool IsUp(MouseButton mouseButton)
    {
        var state = MouseButtonStates[mouseButton];

        return !state.Current && state.Previous;
    }

    internal static void InitializeInput()
    {
        if (WasInitialized)
            return;

        foreach (var key in Enum.GetValues(typeof(Key)))
            KeyStates.Add((Key)key, new ButtonState());

        foreach (var mouseButton in Enum.GetValues(typeof(MouseButton)))
            MouseButtonStates.Add((MouseButton)mouseButton, new ButtonState());

        WasInitialized = true;
    }

    internal static void SetKeyState(Key key, ButtonStateData buttonStateData)
    {        
        SetButtonState(KeyStates[key], buttonStateData);
    }

    internal static void SetMouseButtonState(MouseButton mouseButton, ButtonStateData buttonStateData)
    {
        SetButtonState(MouseButtonStates[mouseButton], buttonStateData);
    }

    internal static void SetButtonState(ButtonState buttonState, ButtonStateData buttonStateData)
    {
        buttonState.Current = buttonStateData.current.ToBool();
        buttonState.Previous = buttonStateData.previous.ToBool();
    }

    internal static void SetMousePosition(float x, float y)
    {
        MousePosition = new Vector2(x, y);
    }
}