#pragma once
#include "Renderer.h"
#include "Entity.h"
#include "Camera.h"
#include "DebugRenderer.h"
#include <iostream>
#include <queue>
#include <Grid.h>
#include <cmath>

#include <d3d12.h>
#include <directxtk\SimpleMath.h>

using namespace DirectX;

using namespace std;

class PathFinding;
class TileMap;
class InputManager;
class Window;
class Player;
class GameManager
{
public:
	GameManager(Window* window);
	~GameManager();

	static GameManager* instance;

	InputManager* inputManager;
	Window* window;
	Camera* cam;
	PathFinding* pathFinding;

	igad::Renderer* renderer = nullptr;
	igad::Shader* shader = nullptr;


	vector<GridRow> gridRow;

	vector<Entity*> entities;
	queue<Entity*> markedForDelete;
	queue<Entity*> markedForAdd;

	Entity* MakeBox();
	Entity* MakeHexagon();

	void Update(float dt);
	void Renderer();

	void MarkObjectForDelete(Entity* entity);
	void MarkObjectForAdd(Entity* entity);

	vector<SimpleMath::Vector2> walls = 
	{	
		SimpleMath::Vector2(2,0),SimpleMath::Vector2(2,1),SimpleMath::Vector2(2,2),SimpleMath::Vector2(2,3),SimpleMath::Vector2(2,4),SimpleMath::Vector2(2,5),SimpleMath::Vector2(2,6),SimpleMath::Vector2(2,7),SimpleMath::Vector2(2,8),
		SimpleMath::Vector2(4,9),SimpleMath::Vector2(4,1),SimpleMath::Vector2(4,2),SimpleMath::Vector2(4,3),SimpleMath::Vector2(4,4),SimpleMath::Vector2(4,5),SimpleMath::Vector2(4,6),SimpleMath::Vector2(4,7),SimpleMath::Vector2(4,8),
		SimpleMath::Vector2(6,0),SimpleMath::Vector2(6,1),SimpleMath::Vector2(6,2),SimpleMath::Vector2(6,3),SimpleMath::Vector2(6,4),SimpleMath::Vector2(6,5),SimpleMath::Vector2(6,6),SimpleMath::Vector2(6,7),SimpleMath::Vector2(6,8),
		SimpleMath::Vector2(8,9),SimpleMath::Vector2(8,1),SimpleMath::Vector2(8,2),SimpleMath::Vector2(8,3),SimpleMath::Vector2(8,4),SimpleMath::Vector2(8,5),SimpleMath::Vector2(8,6),SimpleMath::Vector2(8,7),SimpleMath::Vector2(8,8),
		SimpleMath::Vector2(10,0),SimpleMath::Vector2(10,1),SimpleMath::Vector2(10,2),SimpleMath::Vector2(10,3),SimpleMath::Vector2(10,4),SimpleMath::Vector2(10,5),SimpleMath::Vector2(10,6),SimpleMath::Vector2(10,7),SimpleMath::Vector2(10,8)
	};

private:
	void GameLoop(float dt);
	void AddObjects();
	void DeleteObjects();

	void GameGUI();

	vector<Vector2> path;
	vector<SimpleMath::Vector2> pathA;

	Entity* player;
	int index;
};