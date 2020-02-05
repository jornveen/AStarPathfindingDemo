#include "Window.h"



Window::Window()
{
	window = glfwCreateWindow(width, height, gameName, nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &width, &height);
}

Window::Window(char* gameName, int screenWidth, int screenHeight)
{
	window = glfwCreateWindow(screenWidth, screenHeight, gameName, nullptr, nullptr);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	this->gameName = gameName;
	SetScreenSize(screenWidth, screenHeight);
}


Window::~Window()
{
	
}

void Window::SetScreenSize(int screenWidth, int screenHeight)
{
	this->height = screenHeight;
	this->width = screenWidth;
}
