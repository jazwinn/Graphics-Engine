#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Windows/window.h"
#include "Shader.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Model.h"
#include "DrawShapes.h"
#include "Camera.h"


namespace graphic {

	struct GraphicConfig {
		bool backFaceCulling = false; // Enable backface culling
	};

	class GraphicPipeline
	{
	public:
		GraphicPipeline(window::Window& window);
		GraphicPipeline() = delete;
		~GraphicPipeline();

		
		void StartFrame();
		void EndFrame();
		void Update(float dt);
		void Draw();


		camera::Camera& Camera() { return m_camera; }
		shapes::Shapes& Shapes() { return m_Shapes; }
		Shader& GetShader() { return m_genericShader; }
		Shader& GetInstancedShader() { return m_InstancedShader; }


		GraphicConfig config;
	
	private:

		Shader m_genericShader;
		Shader m_InstancedShader;
		window::Window& m_window;
		shapes::Shapes m_Shapes;
		camera::Camera m_camera;
		

	};
}


