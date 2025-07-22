#include <GLFW/glfw3.h>
#include "Include/Engine.hpp"

#include <iostream>

int main()
{
	if (!engine::initWindow(800, 600, "Vulkan")) 
		return -1;
	
	try 
	{
		engine::initEngine();

		engine::loadTexture("testAssets/takuji.jpg");
		Camera camera({0.0f, 2.0f, 5.0f}, -50.0f, 0.0f, 90.0f);
		engine::createObject();
		
		engine::createPipeline("shaders/vert.spv", "shaders/frag.spv");
		engine::prepareRender();
		
		float speed = 5.0f;
		float lastTime = 0.0f;

		while (!engine::windowShouldClose())
		{
			engine::pollEvents();

			float currentTime = glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			if (engine::up() == GLFW_PRESS)
			{
				camera.moveForward(speed * deltaTime);
			}
			if (engine::down() == GLFW_PRESS)
			{
				camera.moveForward(-speed * deltaTime);
			}
			if (engine::left() == GLFW_PRESS)
			{
				camera.moveRight(-speed * deltaTime);
			}
			if (engine::right() == GLFW_PRESS)
			{
				camera.moveRight(speed * deltaTime);
			}

			engine::render(camera);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		return -1;
	}

	return 0;
}