#include "../Include/Window.hpp"

Window::Window(uint32_t w, uint32_t h, const std::string& t) : width_(w), height_(h), title_(t)
{
	if (!createWindow())
	{
		throw std::runtime_error("createWindow error !");
	}
}
Window::~Window() noexcept
{
	if (window_) glfwDestroyWindow(window_);
	glfwTerminate();
}

bool Window::createWindow()
{
	if (!glfwInit())
	{
		std::cerr << "Error glfw init\n";
		return false;
	}

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