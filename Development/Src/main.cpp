#include "Windows/window.h"
#include "GraphicManager/GraphicPipeline.h"
#include <filesystem>
#include "GraphicManager/Model.h"

#include "GraphicManager/Light.h"

std::vector<Vertex> verticesAABB =
{
	// -X Face
	{{-0.5f,-0.5f,-0.5f}, {-1,0,0}, {1,1,1,1}, {0,0}},
	{{-0.5f, 0.5f,-0.5f}, {-1,0,0}, {1,1,1,1}, {0,1}},
	{{-0.5f, 0.5f, 0.5f}, {-1,0,0}, {1,1,1,1}, {1,1}},
	{{-0.5f,-0.5f, 0.5f}, {-1,0,0}, {1,1,1,1}, {1,0}},

	// +X Face
	{{0.5f,-0.5f,-0.5f}, {1,0,0}, {1,1,1,1}, {0,0}},
	{{0.5f, 0.5f,-0.5f}, {1,0,0}, {1,1,1,1}, {0,1}},
	{{0.5f, 0.5f, 0.5f}, {1,0,0}, {1,1,1,1}, {1,1}},
	{{0.5f,-0.5f, 0.5f}, {1,0,0}, {1,1,1,1}, {1,0}},

	// -Y Face
	{{-0.5f,-0.5f, 0.5f}, {0,-1,0}, {1,1,1,1}, {0,0}},
	{{ 0.5f,-0.5f, 0.5f}, {0,-1,0}, {1,1,1,1}, {1,0}},
	{{ 0.5f,-0.5f,-0.5f}, {0,-1,0}, {1,1,1,1}, {1,1}},
	{{-0.5f,-0.5f,-0.5f}, {0,-1,0}, {1,1,1,1}, {0,1}},

	// +Y Face
	{{-0.5f, 0.5f,-0.5f}, {0,1,0}, {1,1,1,1}, {0,0}},
	{{ 0.5f, 0.5f,-0.5f}, {0,1,0}, {1,1,1,1}, {1,0}},
	{{ 0.5f, 0.5f, 0.5f}, {0,1,0}, {1,1,1,1}, {1,1}},
	{{-0.5f, 0.5f, 0.5f}, {0,1,0}, {1,1,1,1}, {0,1}},

	// -Z Face
	{{-0.5f,-0.5f,-0.5f}, {0,0,-1}, {1,1,1,1}, {0,0}},
	{{ 0.5f,-0.5f,-0.5f}, {0,0,-1}, {1,1,1,1}, {1,0}},
	{{ 0.5f, 0.5f,-0.5f}, {0,0,-1}, {1,1,1,1}, {1,1}},
	{{-0.5f, 0.5f,-0.5f}, {0,0,-1}, {1,1,1,1}, {0,1}},

	// +Z Face
	{{-0.5f,-0.5f, 0.5f}, {0,0,1}, {1,1,1,1}, {0,0}},
	{{ 0.5f,-0.5f, 0.5f}, {0,0,1}, {1,1,1,1}, {1,0}},
	{{ 0.5f, 0.5f, 0.5f}, {0,0,1}, {1,1,1,1}, {1,1}},
	{{-0.5f, 0.5f, 0.5f}, {0,0,1}, {1,1,1,1}, {0,1}},
};

std::vector<GLuint> indicesAABB = {
	// -X face
	0, 1, 2,
	0, 2, 3,

	// +X face
	4, 5, 6,
	4, 6, 7,

	// -Y face
	8, 9, 10,
	8, 10, 11,

	// +Y face
	12, 13, 14,
	12, 14, 15,

	// -Z face
	16, 17, 18,
	16, 18, 19,

	// +Z face
	20, 21, 22,
	20, 22, 23
};

std::vector<Vertex> planeVertices = {
	// Bottom-left
	{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1,1,1,1}, {0.0f, 0.0f}},

	// Bottom-right
	{{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1,1,1,1}, {1.0f, 0.0f}},

	// Top-right
	{{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1,1,1,1}, {1.0f, 1.0f}},

	// Top-left
	{{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1,1,1,1}, {0.0f, 1.0f}}
};


std::vector<GLuint> planeIndices = {
0, 1, 2,  // first triangle
2, 3, 0   // second triangle
};

int main() {
	window::Window window(1600, 800);
	graphic::GraphicPipeline graphics(window);

	Light light(graphics.GetLightShader(),SPOT , verticesAABB, indicesAABB);
	light.SetPosition({ 0.f,0.f,5.f });
	light.SetColor({1.f,0.f,0.f,1.f});


	std::vector<Texture> texture = { Texture("../Resource/diffuse.jpg",DIFFUSE,0), Texture("../Resource/specular.jpg", SPECULAR, 1)};
	Mesh testmesh(planeVertices, planeIndices, texture);
	

	Model model2("../Resource/car.blend");

	auto start = std::chrono::high_resolution_clock::now(); // Start timer
	float lastTime = static_cast<float>(glfwGetTime());
	float deltaTime = 0.0f;
	float lastAlgorithmRunTime = 0.0f;
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		std::string FPS = std::to_string(1.0 / deltaTime);
		std::string newTitle = "Program - FPS:" + FPS;
		glfwSetWindowTitle(window.GetWindow(), newTitle.c_str());

		window.Startframe();
		graphics.StartFrame();

		light.SetPosition(light.GetPosition() + glm::vec3{0.f, 1.f, 0.f} * deltaTime);

		//graphics.Shapes().DrawCube({ 0,0,0 }, { 0,0,0 }, { 50,50,50 });
		//graphics.Shapes().DrawCube({ 50,0,0 }, { 0,0,0 }, { 50,50,50 });
		//graphics.Shapes().DrawCube({ 100,0,0 }, { 0,0,0 }, { 50,50,50 });


		//graphics.Shapes().DrawSphere(shapes::Sphere({ 0,20,0 }, 30));
		//graphics.Shapes().DrawSphere(shapes::Sphere({ 0,0,20 }, 30));


		//shapes::Plane plane;
		//plane.d = 0;
		//plane.normal = glm::vec3(0.0f, 1.0f, 0.0f);

		////// Draw a 5x5 plane with red color
		//graphics.Shapes().DrawPlane(plane, 100.0f, 100.0f);


		model2.Draw(graphics.GetShader(), graphics.Camera());


		graphics.Update(deltaTime);
		//light.Draw(graphics.GetShader(), graphics.Camera());


		//graphics.GetLightShader().setUniform("lightColor", glm::vec4{ 1.f,0.f,0.f,1.f });
		
		graphics.Draw();

		
		

		graphics.EndFrame();
		window.Endframe();
	}


}