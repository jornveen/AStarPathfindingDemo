#pragma once
#include <d3d12.h>
#include <directxtk\SimpleMath.h>
#include "Renderer.h"

using namespace igad;
using namespace DirectX;

struct Transform
{
	SimpleMath::Vector3 position;
	SimpleMath::Vector3 rotation;
	SimpleMath::Vector3 scale;
};

struct MeshRenderer
{
	Texture* texture = nullptr;
	Mesh* mesh = nullptr;
};

class Entity
{
public:
	Entity(std::string tag = "Untagged");
	Entity(SimpleMath::Vector3 pos,
		SimpleMath::Vector3 rot,
		SimpleMath::Vector3 scale,
		std::string texture,
		std::string model,
		std::string tag);
	virtual ~Entity();

	Transform transform;
	MeshRenderer meshRenderer;
	std::string tag;

	bool isActive = true;

	virtual void Start();
	virtual void Update(float dt);
	virtual void Render(Renderer* renderer);

	void DrawNormal();

protected:
	Matrix44 model;
	void CreateModel(SimpleMath::Vector3 pos, SimpleMath::Vector3 rot, SimpleMath::Vector3 scale, std::string texture, std::string model);
};

