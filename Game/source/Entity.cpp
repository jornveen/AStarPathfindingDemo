#include "Entity.h"
#include "DebugRenderer.h"
#include "ExMath.h"


Entity::Entity(std::string tag)
{
	model = Matrix44::CreateIdentity();
	this->transform.scale = SimpleMath::Vector3(1, 1, 1);
	this->tag = tag;
}

Entity::Entity(SimpleMath::Vector3 pos, SimpleMath::Vector3 rot, SimpleMath::Vector3 scale, std::string texture, std::string model, std::string tag)
{
	CreateModel(pos, rot, scale, texture, model);
	this->tag = tag;
}

Entity::~Entity()
{
}

void Entity::Start()
{

}

void Entity::Update(float dt)
{
	model.SetEulerAxis(transform.rotation.x, transform.rotation.y, transform.rotation.z);
	model = model * model.CreateScale(ExMath::SimpleToIgad(transform.scale)/2) ;
	model.SetTranslation(ExMath::SimpleToIgad(transform.position));
}

void Entity::Render(Renderer* renderer)
{
	if (renderer!= nullptr && meshRenderer.mesh != nullptr)
		renderer->Render(model, meshRenderer.texture, meshRenderer.mesh);
}

void Entity::CreateModel(SimpleMath::Vector3 pos, SimpleMath::Vector3 rot, SimpleMath::Vector3 scale, std::string texture, std::string model)
{
	isActive = true;

	this->model = Matrix44::CreateIdentity();
	transform.position = pos;
	transform.rotation = rot;
	transform.scale = scale;

	this->meshRenderer.mesh = new Mesh(model);
	this->meshRenderer.texture = new Texture(texture);
	Update(0);
}

void Entity::DrawNormal()
{
	if (this->meshRenderer.mesh == nullptr)
		return;

	std::vector<VertexFormat> vertexFormat = meshRenderer.mesh->GetVertices();

	for (int i = 0; i < vertexFormat.size(); ++i)
	{
		igad::gDebugDraw3D.AddLine(ExMath::SimpleToIgad(transform.position), ExMath::SimpleToIgad(transform.position) + vertexFormat[i].Normal, igad::Color::Yellow);
	}
}



