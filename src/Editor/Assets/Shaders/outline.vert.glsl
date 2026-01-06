#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model, view, projection;
uniform float thicknessWS; // e.g. 0.03 (world units)

void main() {
    mat3 nrmXform = mat3(transpose(inverse(model)));
    vec3 worldN = normalize(nrmXform * aNormal);
    vec4 worldPos = model * vec4(aPos, 1.0);

    worldPos.xyz += worldN * thicknessWS; // extrude along normal
    gl_Position = projection * view * worldPos;
}