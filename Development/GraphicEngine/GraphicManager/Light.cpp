#include "Light.h"

Light::Light(Shader& lightShader, const std::vector<Vertex>& vboData, const std::vector<GLuint>& eboData, const std::vector<Texture>& texture):
	m_mesh(vboData, eboData, texture),
	m_lightShader(lightShader)
{

}

void Light::Draw(Shader& shader, camera::Camera& camera)
{
	glm::mat4 objectModel = glm::mat4(1.0f);
	objectModel = glm::translate(objectModel, m_position);

	m_lightShader.setUniform("lightColor", m_color);
	m_mesh.Draw(m_lightShader, camera, objectModel);


	shader.setUniform("lightColor", m_color);
	shader.setUniform("lightPos", m_position); 
	

}
