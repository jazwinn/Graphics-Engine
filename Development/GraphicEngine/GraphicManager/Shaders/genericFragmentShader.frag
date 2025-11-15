#version 330 core

in vec3 crntPos;
in vec3 Normal;
in vec4 color;
in vec2 texCoord;


out vec4 FragColor;

uniform vec4 uniform_color;

void main(){

	FragColor = color;

}