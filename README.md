# TRPGEngine

TRPGEngine is a little tactical rpg engine made in C++ with the Vulkan graphics API. For now, you can only draw multiple triangles. 

![TRPGEngine triangles](https://github.com/user-attachments/assets/e477fd6b-c443-4723-bcf2-ebbb2266904e)

```cpp
#include "../Include/Engine.hpp"

#include <iostream>

int main()
{
	if (!engine::initWindow(800, 600, "Vulkan"))
	{
		return -1;
	}
	
	try 
	{
		engine::initEngine();
		engine::createPipeline("shaders/vert.spv", "shaders/frag.spv");
		engine::prepareRender();

		while (!engine::windowShouldClose())
		{
			engine::pollEvents();
			engine::render();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		return -1;
	}

	return 0;
}
```
