#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GameManager.h"
#include "Window.h"
#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"
#include <iostream>

using namespace std;

GameManager* gameManager;
Window* window;

int main()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);		
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	if (!glfwInit())
		exit(EXIT_FAILURE);


	window = new Window();

	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	auto lastFrame = glfwGetTime();

	gameManager = new GameManager(window);
	glViewport(0, 0, window->width, window->height);

	ImGui_ImplGlfwGL3_Init(window->window, true);

	ImGui::StyleColorsClassic();

	while (!glfwWindowShouldClose(window->window))
	{
		auto currentFrame = glfwGetTime();
		float dt = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.5, 0.5, 1.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);                           

		gameManager->Update(dt);
		gameManager->Renderer();

		glfwSwapBuffers(window->window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window->window);
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}