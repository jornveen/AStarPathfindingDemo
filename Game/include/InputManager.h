#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Vector3.h"

class Window;
class InputManager
{
public:
	InputManager(Window* window);
	~InputManager();

	Window* window = nullptr;

	bool KeyPressed(int key);
	bool KeyUp(int key);
	bool KeyDown(int key);
	float CheckAxis(int key1, int key2);
	float CheckAxisKeyUp(int key1, int key2);
	float CheckAxisKeyDown(int key1, int key2);
	igad::Vector2 GetMouseDir(igad::Vector2 lastMousePos, igad::Vector2 curMousePos);
	igad::Vector2 GetMousePos();

private:
	bool keys[350];
};

