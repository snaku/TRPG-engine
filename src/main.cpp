#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include <Core/Engine.hpp>
#include <Core/Camera.hpp>
#include <Scene/GameObject.hpp>
#include <Scene/ShapeType.hpp>
#include <Scene/Scene.hpp>
#include <Scene/TransformComponents.hpp>

// std
#include <iostream>
#include <chrono>

/*const int gridSize = 20; // 20 x 20 x 20 = 8000 objects
	const float spacing = 2.0f;
	float xSpacing = 1.0;

	for (int x = 0; x < gridSize; ++x)
	{
		for (int y = 0; y < gridSize; ++y)
		{
			for (int z = 0; z < gridSize; ++z)
			{
				// std::string name = "obj_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);
				engine::GameObject& obj = scene.createGameObject("object");
				TransformComponent* transform = obj.addComponent<TransformComponent>();
				transform->position = { x + xSpacing, y + xSpacing, z * spacing };
			}
		}
	}*/

void processInput(Camera& camera, float deltaTime)
{
	float cameraMovementSpeed = 7.5f;
	float cameraRotationSpeed = 70.0f;

	if (engine::shiftKey() == GLFW_PRESS)
	{
		cameraMovementSpeed *= 2.0f;
		cameraRotationSpeed *= 2.0f;
	}

	// ZQSD keys
	if (engine::up() == GLFW_PRESS)
		camera.moveForward(cameraMovementSpeed * deltaTime);
	
	if (engine::down() == GLFW_PRESS)
		camera.moveForward(-cameraMovementSpeed * deltaTime);
	
	if (engine::left() == GLFW_PRESS)
		camera.moveRight(-cameraMovementSpeed * deltaTime);
	
	if (engine::right() == GLFW_PRESS)
		camera.moveRight(cameraMovementSpeed * deltaTime);
	
	// arrow keys
	if (engine::upArrow() == GLFW_PRESS) 
		camera.rotateX(cameraRotationSpeed * deltaTime);

	if (engine::downArrow() == GLFW_PRESS)
		camera.rotateX(-cameraRotationSpeed * deltaTime);

	if (engine::leftArrow() == GLFW_PRESS)
		camera.rotateY(-cameraRotationSpeed * deltaTime);
	
	if (engine::rightArrow() == GLFW_PRESS)
		camera.rotateY(cameraRotationSpeed * deltaTime);		
}

int main(int argc, char** argv)
{
	if (!engine::initWindow(1280, 720, "Vulkan")) 
		return -1;
	
	Camera camera({18.0f, 10.0f, 17.5f}, -55.0f, -35.0f, 45.0f);
	camera.setType(Camera::Type::Isometric);
	// camera.zoom(1.75f);

	engine::Scene scene;

	const int gridSize = 20;
	const float spacing = 2.0f;
	float xSpacing = 1.0;

	for (int x = 0; x < gridSize; ++x)
	{
		for (int z = 0; z < gridSize; ++z)
		{
			// std::string name = "obj_" + std::to_string(x) + "_" + std::to_string(y) + "_" + std::to_string(z);
			engine::GameObject& obj = scene.createGameObject("object", ShapeType::CUBE_3D);
			TransformComponent* transform = obj.addComponent<TransformComponent>();
			transform->position = { x + xSpacing, 0.0f, z + xSpacing };
		}
	}

	try 
	{
		engine::initEngine();

		auto start = std::chrono::high_resolution_clock::now();

		for (const auto& renderable : scene.getRenderableData())
		{
			engine::createMesh(renderable);
		}

		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> duration = end - start;

		std::cout << "Creating " << scene.getGameObjects().size() << " GameObjects took " << duration.count() << " secondes" << std::endl;

		engine::loadTexture("testAssets/takuji.jpg");
		
		engine::createPipeline("shaders/vert.spv", "shaders/frag.spv");
		engine::prepareRender();

		float lastTime = 0.0f;
		float fpsLastTime = lastTime;
		int nbFrames = 0;

		while (!engine::windowShouldClose())
		{
			engine::pollEvents();

			float currentTime = glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			nbFrames++;

			if (currentTime - fpsLastTime >= 1.0f)
			{
				std::cout << "FPS: " << nbFrames << std::endl;
				nbFrames = 0;
				fpsLastTime = currentTime;
			}

			processInput(camera, deltaTime);

			scene.update(deltaTime);

			engine::render(scene, camera);
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		return -1;
	}

	return 0;
}