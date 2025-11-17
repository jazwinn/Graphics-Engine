#pragma once

#include <glad/glad.h>
#include "Shader.h"

enum TextureType {
	NONE = 0,
	DIFFUSE,
	SPECULAR,
	NORMAL
};

class Texture
{

public:
	GLuint ID;
	TextureType type = NONE;
	GLuint unit;
	Texture(const char* image, TextureType texType, GLuint slot);

	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	void Bind();
	void Unbind();
	void Delete();

	//char* GetData();
};

