#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include<string>
#include<iostream>


class Window {
public:
	Window(int width, int height, std::string titel);
	bool shouldClose();
	void swapBuffers();
	void pollEvents();
	void terminate();
public:
	GLFWwindow* handel;
private:
	std::string titel;
	int width, height;
};