#pragma once
#include "Entity.h"

class InputManager;
class Camera : public Entity
{
public:
	Camera();
	Camera(float fov, float screenWidth, float screenHeight,float nearZ, float farZ);
	~Camera();

	igad::Matrix44 view;
	igad::Matrix44 projection;

	void Update(float dt) override;
	void UpdateView();

	float camSpeed = 5;

private:
	InputManager* inputManager;
	igad::Vector2 lastMousePos;
};


