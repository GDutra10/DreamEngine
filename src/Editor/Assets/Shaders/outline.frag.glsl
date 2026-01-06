#version 330 core
out vec4 FragColor;

uniform vec3 outlineColor;

void main()
{
    FragColor = vec4(outlineColor.x, outlineColor.y, outlineColor.z, 1.0);
}