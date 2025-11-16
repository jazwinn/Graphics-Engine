#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;


out vec3 crntPos;
out vec3 Normal;
out vec4 color;
out vec2 texCoord;

uniform mat4 camVP;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 model;

void main() {
    crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
    Normal = aNormal;
    color = vec4(aColor,1.f);
    texCoord = aTex;

    gl_Position = camVP * vec4(crntPos, 1.0);
}