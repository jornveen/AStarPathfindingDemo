#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

class Window
{
public:
	Window();
	Window(char* gameName, int screenWidth, int screenHeight);
	~Window();

	char* gameName = "Game";
	int height = 720;
	int width = 1280;
	GLFWwindow* window;

	void SetScreenSize(int screenWidth, int screenHeight);
};

