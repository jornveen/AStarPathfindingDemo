#include "Camera.h"
#include "GameManager.h"
#include "InputManager.h"
#include "ExMath.h"

Camera::Camera()
{
	this->projection = igad::Matrix44::CreatePerspective(igad::DegToRad(60), 16.0f / 9.0f, 0.1f, 2000.0f);
	inputManager = GameManager::instance->inputManager;
	transform.position = SimpleMath::Vector3(0, 0, 20);
}

Camera::Camera(float fov, float screenWidth, float screenHeight, float nearZ, float farZ)
{
	this->projection = igad::Matrix44::CreatePerspective(igad::DegToRad(fov), screenWidth / screenHeight, nearZ, farZ);
	inputManager = GameManager::instance->inputManager;
	transform.position = SimpleMath::Vector3(0, 0, 20);
}


Camera::~Camera()
{
}

void Camera::Update(float dt)
{
	igad::Vector2 mousePos = inputManager->GetMousePos();
	igad::Vector2 mouseDir = inputManager->GetMouseDir(lastMousePos, mousePos);
	lastMousePos = mousePos;

	//transform.rotation.x -= mouseDir.x * dt;
	//transform.rotation.y -= mouseDir.y * dt;

	transform.position.x -= inputManager->CheckAxis(GLFW_KEY_LEFT, GLFW_KEY_RIGHT) * dt * camSpeed;
	transform.position.y += inputManager->CheckAxis(GLFW_KEY_UP, GLFW_KEY_DOWN) * dt * camSpeed;
	transform.position.z += inputManager->CheckAxis(GLFW_KEY_O, GLFW_KEY_P) * dt * camSpeed;

	UpdateView();
}

void Camera::UpdateView()
{
	model.SetEulerAxis(transform.rotation.x, transform.rotation.y, transform.rotation.z);
	igad::Vector3 camDir = model * Vector3(0, 0, -1);
	camDir.Normalize();
	view = igad::Matrix44::CreateLookAt(ExMath::SimpleToIgad(transform.position), ExMath::SimpleToIgad(transform.position) + camDir, Vector3(0, 1, 0));
}