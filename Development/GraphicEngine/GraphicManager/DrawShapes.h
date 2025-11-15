#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>


#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Shapes/shapes.h"


namespace shapes {

	enum DrawType {
		FILLED,
		WIREFRAME,
		FILLEDWIREFRAME
	};

	struct InstancedShape {
		std::vector<glm::mat4> transformVector;
		std::vector<glm::vec4> color;
	};


	class Shapes
	{
	private:
		camera::Camera& m_camera;
		Shader& m_Shader;
		Shader& m_InstancedShader;
	public:
		Shapes(camera::Camera& camera, Shader& shader, Shader& instancedShader);

		

		void DrawCube(const glm::vec3& center, const glm::vec3& rotation, const glm::vec3& scale, const glm::vec4& color = {1.f,1.f,1.f,1.f});
		void DrawCube(const shapes::Aabb& aabb, const glm::vec4& color = { 1.f,1.f,1.f,1.f });
		void DrawSphere(const shapes::Sphere& sphere, const glm::vec4& color = { 1.f,1.f,1.f,1.f });
		void DrawPlane(const shapes::Plane& plane, float width, float height, const glm::vec4& color = { 1.f,1.f,1.f,1.f });


		void Draw();
	private:
		enum ShapeType
		{
			AABB,
			SPHERE,
			PLANE,
			LINE
		};

		std::unordered_map<ShapeType, InstancedShape> m_shapeData;
		std::unordered_map<ShapeType, std::unique_ptr<Mesh>> m_shapeInstance;



	private: //temp not using for now
		//void Draw_Rectangle(const glm::mat4& vp, const glm::vec3 center, const glm::vec3 scale, const glm::vec4& color, DrawType drawtype = FILLED, const glm::vec4& wireFrameColor = { 0.f,0.f,0.f,1.f });
		void Draw_Line(const glm::mat4& vp, const glm::vec3 start, const glm::vec3 end, const glm::vec4& color);

		std::unique_ptr<Mesh> m_RectangleMesh;
		std::unique_ptr<Mesh> m_LineMesh;
	};
}


