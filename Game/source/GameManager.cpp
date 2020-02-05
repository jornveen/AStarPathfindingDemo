#include "GameManager.h"
#include "Window.h"
#include "InputManager.h"
#include "Mesh.h"
#include "PathFinding.h"
#include <ExMath.h>

#include <imgui.h>
#include "imgui_impl_glfw_gl3.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager(Window* window)
{
	instance = this;
	this->inputManager = new InputManager(window);
	this->window = window;
	this->cam = new Camera();

	this->shader = new igad::Shader("./Assets/Shaders/Basic3D.vsh", "./Assets/Shaders/Basic3D.fsh");
	this->renderer = new igad::Renderer(shader);
	this->pathFinding = new PathFinding();
	

	gridRow = vector<GridRow>(20);

	for (int x = 0; x < 20; ++x)
	{
		gridRow[x].gridCollum = vector<GridCollum>(10);
	}
	

	for (size_t x = 0; x < gridRow.size(); ++x)
	{
		for (size_t y = 0; y < gridRow[x].gridCollum.size(); ++y)
		{
			TileInfo tileInfo;
			tileInfo.tileHeight = 1;

			tileInfo.tile = MakeBox();

			if (y % 2 == 0)
				tileInfo.tile->transform.position = SimpleMath::Vector3(x, y, 1);		
			else			
				tileInfo.tile->transform.position = SimpleMath::Vector3(x + 0.5f, y, 1);
			

			if (pathFinding->IsInVector(walls, SimpleMath::Vector2(x, y)))
			{
				tileInfo.tileType = TileType::UnClaimedTile;
				tileInfo.tileMovementCost = 5;
				tileInfo.tile->meshRenderer.texture = new Texture("./Assets/Textures/Black.png");
			}
			else
			{
				tileInfo.tileType = TileType::ClaimedTile;
				tileInfo.tile->meshRenderer.texture = new Texture("./Assets/Textures/Blue.png");
			}

			MarkObjectForAdd(tileInfo.tile);
			gridRow[x].gridCollum[y].tiles = tileInfo;
		}
	}

	pathFinding->SetGrid(gridRow);


	pathA = pathFinding->GetPathA(SimpleMath::Vector2(1, 1), SimpleMath::Vector2(18, 5),GridType::HEXAGON);


	player = MakeBox();
	player->transform.position = SimpleMath::Vector3(1, 1, 1.1f);
	MarkObjectForAdd(player);
	gDebugDraw3D.Initialize();
}


GameManager::~GameManager()
{
	
}

Entity * GameManager::MakeBox()
{
	Entity* box = new Entity(
		SimpleMath::Vector3(),
		SimpleMath::Vector3(),
		SimpleMath::Vector3(1.8f/2, 1.56f/2, 0.1f),
		"./Assets/Textures/white.png",
		"./Assets/Models/Box.obj",
		"Untagged");

	return box;
}

Entity * GameManager::MakeHexagon()
{
	Entity* hexagon = new Entity(
		SimpleMath::Vector3(),
		SimpleMath::Vector3(),
		SimpleMath::Vector3(1.8f, 1.56f, 1),
		"./Assets/Textures/white.png",
		"./Assets/Models/hexagon.obj",
		"Untagged");

	return hexagon;
}

void GameManager::Update(float dt)
{
	AddObjects();

	GameLoop(dt);

	cam->Update(dt);
}

void GameManager::GameLoop(float dt)
{
	for (size_t i = 0; i < entities.size(); ++i)
	{
		if (entities[i] != nullptr && entities[i]->isActive)
		{
			entities[i]->Update(dt);
		}
	}

	//Vector3 tilePos = gridRow[(int)pathA[index].x].gridCollum[(int)pathA[index].y].tiles.tile->transform.position;
	//tilePos.z = 1.1f;
	//player->transform.position = ExMath::Lerp(player->transform.position, tilePos,dt * 10);
	if (inputManager->KeyDown(GLFW_KEY_B))
	{
		index++;
	}

}

void GameManager::Renderer()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	renderer->Begin(cam->view, cam->projection);

	for (size_t i = 0; i < entities.size(); ++i)
	{
		if (entities[i] != nullptr && entities[i]->isActive)
		{
			entities[i]->Render(renderer);
		}
	}


	renderer->End();

	igad::gDebugDraw3D.Draw(cam->projection * cam->view);
	gDebugDraw3D.Clear();

	DeleteObjects();
}

void GameManager::MarkObjectForAdd(Entity* entity)
{
	markedForAdd.push(entity);
}

void GameManager::AddObjects()
{
	while(!markedForAdd.empty())
	{
		entities.push_back(markedForAdd.front());
		markedForAdd.pop();
	}
}


void GameManager::MarkObjectForDelete(Entity* entity)
{
	markedForDelete.push(entity);
}

void GameManager::DeleteObjects()
{
	int indexDeletedObjects = markedForDelete.size();
	while (!markedForDelete.empty())
	{
		for (size_t i = 0; i < entities.size();++i)
		{
			if (markedForDelete.front() == entities[i])
			{
				entities[i] = nullptr;
				delete entities[i];
				entities.erase(entities.begin() + i);
			}
		}
		markedForDelete.pop();
	}
}

void GameManager::GameGUI()
{
	/*ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Text("Game Options");
	
	ImGui::Text("Game Controls");
	ImGui::BulletText("Move the player with sd.");
	ImGui::BulletText("Jump with space.");
	ImGui::Render();*/
}