#include "../Include/Window.hpp"
#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanInstance.hpp"
#include "../Include/VulkanDevice.hpp"

int main()
{
	try 
	{
		Window window(800, 600, "test window");
		
		VulkanContext ctx;
		VulkanInstance vulkanInstance(ctx, window);
		VulkanDevice vulkanDevice(ctx);

		GLFWwindow* glfwWindow = window.getWindow();
		while (!glfwWindowShouldClose(glfwWindow))
		{
			glfwPollEvents();
		}
	}
	catch (const std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
		return -1;
	}

	return 0;
}