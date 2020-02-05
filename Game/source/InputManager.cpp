#include "InputManager.h"
#include "Window.h"


InputManager::InputManager(Window* window)
{
	this->window = window;
}


InputManager::~InputManager()
{
}


igad::Vector2 InputManager::GetMouseDir(igad::Vector2 lastMousePos, igad::Vector2 curMousePos)
{
	igad::Vector2 result = lastMousePos - curMousePos;
	result.Normalize();
	return result;
}

igad::Vector2 InputManager::GetMousePos()
{
	double x,y;
	glfwGetCursorPos(window->window, &x, &y);
	return igad::Vector2(x,y);
}


float InputManager::CheckAxis(int key1, int key2)
{
	float result = 0;
	if (KeyPressed(key1))
	{
		result++;
	}

	if (KeyPressed(key2))
	{
		result--;
	}

	return result;
}

float InputManager::CheckAxisKeyUp(int key1, int key2)
{
	float result = 0;
	if (KeyUp(key1))
	{
		result++;
	}

	if (KeyUp(key2))
	{
		result--;
	}

	return result;
}

float InputManager::CheckAxisKeyDown(int key1, int key2)
{
	float result = 0;
	if (KeyDown(key1))
	{
		result++;
	}

	if (KeyDown(key2))
	{
		result--;
	}

	return result;
}


bool InputManager::KeyPressed(int key)
{
	return glfwGetKey(window->window, key) == GLFW_PRESS;
}

bool InputManager::KeyDown(int key)
{
	if (keys[key] == false && glfwGetKey(window->window, key) == GLFW_PRESS)
	{
		keys[key] = true;
		return true;
	}
	else if (keys[key] == true && glfwGetKey(window->window, key) != GLFW_PRESS)
	{
		keys[key] = false;
	}
	return false;
}

bool InputManager::KeyUp(int key)
{
	if (keys[key] == false && glfwGetKey(window->window, key) == GLFW_PRESS)
	{
		keys[key] = true;
	}
	else if (keys[key] == true && glfwGetKey(window->window, key) != GLFW_PRESS)
	{
		keys[key] = false;
		return true;
	}
	return false;
}