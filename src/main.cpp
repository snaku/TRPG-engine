#include <GLFW/glfw3.h>
#include "Include/Engine.hpp"
#include "Include/GameObject.hpp"
#include "Include/Scene.hpp"
#include "Include/TransformComponents.hpp"

#include <iostream>

void processInput(Camera& camera, float deltaTime)
{
	float cameraMovementSpeed = 7.5f;
	float cameraRotationSpeed = 50.0f;

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
	if (!engine::initWindow(800, 600, "Vulkan")) 
		return -1;
	
	Camera camera({0.0f, 10.0f, 10.0f}, -55.0f, -35.0f, 45.0f);
	engine::Scene scene;

	auto& object = scene.createGameObject("object");
	auto* transformComp = object.addComponent<TransformComponent>();

	try 
	{
		engine::initEngine();

		for (const auto& renderable : scene.getRenderableData())
		{
			engine::createMesh(renderable);
		}

		engine::loadTexture("testAssets/takuji.jpg");
		
		engine::createPipeline("shaders/vert.spv", "shaders/frag.spv");
		engine::prepareRender();
		
		float lastTime = 0.0f;

		while (!engine::windowShouldClose())
		{
			engine::pollEvents();

			float currentTime = glfwGetTime();
			float deltaTime = currentTime - lastTime;
			lastTime = currentTime;

			processInput(camera, deltaTime);

			transformComp->scale += 0.1f * deltaTime;

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