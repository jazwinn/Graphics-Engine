#include "Windows/window.h"
#include "GraphicManager/GraphicPipeline.h"
#include <filesystem>
#include "GraphicManager/Model.h"

int main() {
	window::Window window(1600, 800);
	graphic::GraphicPipeline graphics(window);

	Model model("../Resource/untitled.fbx");

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

		model.Draw(graphics.GetShader(), graphics.Camera());

		graphics.Update(deltaTime);
		
		graphics.Draw();

		
		

		graphics.EndFrame();
		window.Endframe();
	}


}