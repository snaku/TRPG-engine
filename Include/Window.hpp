#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window
{
public:
	Window(int w, int h,const std::string& t);
	~Window() noexcept;

	GLFWwindow* getWindow() const { return window_; }
private:
	[[nodiscard]] bool createWindow();

	int height_, width_;
	std::string title_;
	GLFWwindow* window_ = nullptr;
};