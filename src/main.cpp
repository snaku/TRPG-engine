#include "../Include/Window.hpp"
#include "../Include/VulkanContext.hpp"
#include "../Include/VulkanInstance.hpp"
#include "../Include/VulkanDevice.hpp"
#include "../Include/VulkanSwapchain.hpp"
#include "../Include/VulkanRenderPass.hpp"
#include "../Include/VulkanPipeline.hpp"

int main()
{
	try 
	{
		Window window(800, 600, "test window");
		
		VulkanContext ctx;
		VulkanInstance vulkanInstance(ctx, window);
		VulkanDevice vulkanDevice(ctx);
		VulkanSwapchain swapchain(ctx, vulkanDevice, window);
		VulkanRenderPass renderPass(ctx, swapchain);
		VulkanPipeline vulkanPipeline(ctx);

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