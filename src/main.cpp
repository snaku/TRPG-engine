#include <GLFW/glfw3.h>
#include <glm/gtc/quaternion.hpp>

#include "Include/Engine.hpp"
#include "Include/GameObject.hpp"
#include "Include/ShapeType.hpp"
#include "Include/Scene.hpp"
#include "Include/TransformComponents.hpp"
#include "Include/JumpComponent.hpp"

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

int main()
{
	if (!engine::initWindow(1920, 1080, "Vulkan")) 
		return -1;
	
	Camera camera({0.0f, 10.0f, 10.0f}, -55.0f, -35.0f, 45.0f);

	engine::Scene scene;

	engine::GameObject& cube = scene.createGameObject("cube", ShapeType::CUBE_3D);
	TransformComponent* transformCube = cube.addComponent<TransformComponent>();

	engine::GameObject& parallelogram = scene.createGameObject("parallelogram", ShapeType::PARALLELOGRAM_2D);
	TransformComponent* transformParallelogram = parallelogram.addComponent<TransformComponent>();

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

			glm::vec3 dir = camera.getPosition() - transformParallelogram->position;
			dir.y = 0.0f;
			dir = glm::normalize(dir);

			float angleY = atan2(dir.x, dir.z); 
			float angleYDegrees = glm::degrees(angleY);
			float correctedAngleYDegrees = angleYDegrees + 180.0f;
			if (correctedAngleYDegrees > 360.0f)
				correctedAngleYDegrees -= 360.0f;

			transformParallelogram->rotation = glm::vec3(0.0f, correctedAngleYDegrees, 0.0f);
			transformCube->rotation.y += 30.0f * deltaTime;

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