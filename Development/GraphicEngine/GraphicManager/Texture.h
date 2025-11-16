#pragma once

#include <glad/glad.h>
#include "Shader.h"

enum TextureType {
	DIFFUSE,
	SPECULAR,
	NORMAL
};

class Texture
{

public:
	GLuint ID;
	TextureType type;
	GLuint unit;
	Texture(const char* image, TextureType texType, GLuint slot);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

	//char* GetData();
};

