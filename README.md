# TRPGEngine

TRPGEngine is a little tactical rpg engine made in C++ using the Vulkan graphics API. Still in development !

# Features

* **Camera system** -> Supports **Perspective** and **Isometric** views
* **GameObject and Components** -> Add custom **components** to **objects**
* **Scene management** -> Update multiple **GameObjects**
* **Rendering** -> Render multiple **2D** and **3D** objects
* **Input handling** -> Interact with scene via keyboard

# Code Example

Here is a little snippet of code to render a 20x20 cube plateform with an isometric camera:
```cpp
if (!engine::initWindow(1280, 720, "Vulkan")) return -1;
	
Camera camera({18.0f, 10.0f, 17.5f}, -55.0f, -35.0f, 45.0f);
camera.setType(Camera::Type::Isometric);

engine::Scene scene;

const int gridSize = 20;
for (int x = 0; x < gridSize; ++x)
{
	for (int z = 0; z < gridSize; ++z)
	{
		engine::GameObject& obj = scene.createGameObject("object", ShapeType::CUBE_3D);
		TransformComponent* transform = obj.addComponent<TransformComponent>();
		transform->position = { x + 1, 0.0f, z + 1 };
	}
}

engine::initEngine();

for (const auto& renderable : scene.getRenderableData())
{
	engine::createMesh(renderable);
}
```
