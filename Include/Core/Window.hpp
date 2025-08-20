#pragma once

#include <GLFW/glfw3.h>

// std
#include <string>
#include <iostream>

class Window
{
public:
	Window() = default;
	~Window() noexcept;

	[[nodiscard]] bool createWindow(uint32_t width, uint32_t height, const std::string& title);
	bool windowShouldClose();
	void pollEvents();

	// getters
	GLFWwindow* getWindow() const { return window_; }
	uint32_t getHeight() const { return height_; }
	uint32_t getWidth() const { return width_; }

private:
	uint32_t height_, width_;
	std::string title_;
	GLFWwindow* window_ = nullptr;
};