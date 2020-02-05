#pragma once

#include <Grid.h>
#include <queue>

using namespace DirectX;

enum GridType {HEXAGON, SQUARE};

class PathFinding
{
public:
	PathFinding();
	~PathFinding();

	//Set the grid 
	void SetGrid(vector<GridRow> &gridRow);

	//Get the path
	vector<SimpleMath::Vector2> GetPathSample(SimpleMath::Vector2 startPoint, SimpleMath::Vector2 endPoint, GridType gridType);

	//Get the path
	vector<SimpleMath::Vector2> GetPathA(SimpleMath::Vector2 startPoint, SimpleMath::Vector2 endPoint, GridType gridType);

	//Get distance between tiles
	float DistanceBetweenTiles(SimpleMath::Vector2 tile1, SimpleMath::Vector2 tile2);

	//Check if it is in a the vector
	bool IsInVector(vector<SimpleMath::Vector2> vector, SimpleMath::Vector2 point);

	//Get closest enemy tile
	SimpleMath::Vector2 GetClosestEnemyTile(SimpleMath::Vector2 currentTile, int owner);

	//Get all enemies tiles
	vector<SimpleMath::Vector2> GetAllEnemyTiles(SimpleMath::Vector2 currentTile, int owner);

	//Get distance between two tiles over a path
	float DistanceOfPathBetweenTiles(SimpleMath::Vector2 tile1, SimpleMath::Vector2 tile2, GridType gridType);

	//Get distance of path
	float DistanceOfPath(vector<SimpleMath::Vector2> path);

	//Getting Neighbors
	vector<SimpleMath::Vector2> GetNeighborsHexagon(SimpleMath::Vector2 currenTile);
	vector<SimpleMath::Vector2> GetNeighborsSquare(SimpleMath::Vector2 currenTile);

	int GetNumberOfUnits(int owner);
	int GetNumberOfUnits(int owner, std::string tag);

	vector<SimpleMath::Vector2> GetAllTilesUnitAreOn(int owner);
	vector<SimpleMath::Vector2> GetAllTilesUnitAreOn(int owner, std::string tag);

	int GetNumberOfTiles(int owner);
	int GetNumberOfTiles(TileType tileType);

	vector<SimpleMath::Vector2> GetTiles(int owner);
	vector<SimpleMath::Vector2> GetTiles(TileType tileType);

	int GetNumberOfUnitsOnTile(SimpleMath::Vector2 tile);
	int GetNumberOfUnitsOnTile(SimpleMath::Vector2 tile, std::string tag);

private:

	//Grid
	vector<GridRow> gridRow;

	//Calculate the path
	vector<SimpleMath::Vector2> CalculatePathSample(vector<vector<Vector3>> allPaths, SimpleMath::Vector2 endPoint);

	vector<SimpleMath::Vector2> CalculatePathA(std::map<SimpleMath::Vector2, SimpleMath::Vector2> cameFrom, SimpleMath::Vector2 endPoint, SimpleMath::Vector2 startPoint);
	
	//Get the closest tile
	SimpleMath::Vector2 GetClosestTile(SimpleMath::Vector2 tile, vector<Vector3> tiles);

	//Check a tile at a certain point
	bool CheckTileAtPoint(SimpleMath::Vector2 point);

	float Heuristic(SimpleMath::Vector2 tile1, SimpleMath::Vector2 tile2);

	//Colors
	std::string color[5] = { "./Assets/Textures/Blue.png", "./Assets/Textures/Green.png", "./Assets/Textures/Red.png" , "./Assets/Textures/Yellow.png", "./Assets/Textures/LightBlue.png" };


	//Neighbors 
	vector<SimpleMath::Vector2> evenr = 
	{	
		SimpleMath::Vector2(-1, 0), SimpleMath::Vector2(-1, 1) ,SimpleMath::Vector2(0, 1),
		SimpleMath::Vector2(1, 0), SimpleMath::Vector2(0, -1),SimpleMath::Vector2(-1, -1)
	};

	vector<SimpleMath::Vector2> oddr =
	{ 
		SimpleMath::Vector2(1, 0), SimpleMath::Vector2(1, -1),SimpleMath::Vector2(0, -1),
		SimpleMath::Vector2(-1, 0), SimpleMath::Vector2(0, 1) ,SimpleMath::Vector2(1, 1)
	};

	vector<SimpleMath::Vector2> square =
	{
		SimpleMath::Vector2(1, 0), SimpleMath::Vector2(0, -1), SimpleMath::Vector2(-1, 0), SimpleMath::Vector2(0,1)
	};

	typedef pair<float, SimpleMath::Vector2> tilePriorityElement;

	struct CustomCompare
	{
		bool operator()(const tilePriorityElement& pair1, const tilePriorityElement& pair2)
		{
			return pair1.first > pair2.first;
		}
	};
};