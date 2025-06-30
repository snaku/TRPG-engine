#include "../Include/Window.hpp"

Window::~Window() noexcept
{
	if (window_) glfwDestroyWindow(window_);
	glfwTerminate();
}

bool Window::createWindow(uint32_t width, uint32_t height, const std::string& title)
{
	if (!glfwInit())
	{
		std::cerr << "Error glfw init\n";
		return false;
	}

	width_ = width;
	height_ = height;
	title_ = title;

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // later

	window_ = glfwCreateWindow(width_, height_, title_.c_str(), nullptr, nullptr);
	if (!window_)
	{
		std::cerr << "Error glfw window\n";
		glfwTerminate();
		return false;
	}

	return true;
}

bool Window::windowShouldClose()
{
	return glfwWindowShouldClose(window_);
}

void Window::pollEvents()
{
	glfwPollEvents();
}