#include "Windows/window.h"
#include "GraphicManager/GraphicPipeline.h"
#include <filesystem>

int main() {
	window::Window window(1600, 800);
	graphic::GraphicPipeline graphics(window);

	auto start = std::chrono::high_resolution_clock::now(); // Start timer

	float lastTime = static_cast<float>(glfwGetTime());
	float deltaTime = 0.0f;
	float lastAlgorithmRunTime = 0.0f;
	while (!glfwWindowShouldClose(window.GetWindow())) {
		float currentTime = static_cast<float>(glfwGetTime());
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		window.Startframe();
		graphics.StartFrame();


		//graphics.Shapes().DrawCube({ 0,0,0 }, { 0,0,0 }, { 50,50,50 }, {1,0,0,1});
		//graphics.Shapes().DrawCube({ 50,0,0 }, { 0,0,0 }, { 50,50,50 }, {0,1,0,1});
		//graphics.Shapes().DrawCube({ 100,0,0 }, { 0,0,0 }, { 50,50,50 }, { 0,0,1,1 });

		////graphics.Shapes().DrawCube(shapes::Aabb({ 0,0,0 }, 10, 10, 10));

		//graphics.Shapes().DrawSphere(shapes::Sphere({ 0,200,0 }, 30));

		shapes::Plane plane;
		plane.d = 0;
		plane.normal = glm::vec3(0.0f, 1.0f, 0.0f);

		// Draw a 5x5 plane with red color
		graphics.Shapes().DrawPlane(plane, 100.0f, 100.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));


		graphics.Update(deltaTime);
		
		graphics.Draw();



		graphics.EndFrame();
		window.Endframe();
	}


}