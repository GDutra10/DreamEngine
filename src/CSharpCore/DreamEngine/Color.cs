namespace DreamEngine;
public struct Color
{
    public float Red { get; set; }
    public float Green { get; set; }
    public float Blue { get; set; }

    public Color() {}

    public Color(float red, float green, float blue)
    {
        Red = red;
        Green = green;
        Blue = blue;
    }
}
