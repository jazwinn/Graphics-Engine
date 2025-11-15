#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

layout (location = 4) in mat4x4 ainstancedMatrix;
layout (location = 8) in vec4 RColor;

out vec3 crntPos;
out vec3 Normal;
out vec4 color;
out vec2 texCoord;



uniform mat4 camVP;

void main() {
    crntPos = aPos;
    Normal = aNormal;
    color = RColor;
    texCoord = aTex;

    gl_Position = camVP * ainstancedMatrix * vec4(aPos, 1.0);
}