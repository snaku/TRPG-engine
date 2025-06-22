#pragma once

#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window
{
public:
	Window(uint32_t w, uint32_t h, const std::string& t);
	~Window() noexcept;

	GLFWwindow* getWindow() const { return window_; }
	uint32_t getHeight() const { return height_; }
	uint32_t getWidth() const { return width_; }
private:
	[[nodiscard]] bool createWindow();

	uint32_t height_, width_;
	std::string title_;
	GLFWwindow* window_ = nullptr;
};