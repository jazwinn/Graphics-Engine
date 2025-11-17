#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

uniform mat4 camVP;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform mat4 model;

void main()
{
	vec3 pos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
	gl_Position = camVP * vec4(pos, 1.0);
}