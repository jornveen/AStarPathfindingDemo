#include "..\include\PathFinding.h"
#include "ExMath.h"
#include <iostream>


PathFinding::PathFinding()
{
}

PathFinding::~PathFinding()
{
}

void PathFinding::SetGrid(vector<GridRow> &gridRow)
{
	this->gridRow = gridRow;
}

#pragma region GetPath
vector<SimpleMath::Vector2> PathFinding::GetPathSample(SimpleMath::Vector2 startPoint, SimpleMath::Vector2 endPoint, GridType gridType)
{
	vector<SimpleMath::Vector2> neighbors;
	vector<vector<Vector3>> allPaths;
	vector<SimpleMath::Vector2> visited;
	vector<Vector3> arr;
	vector<Vector3> temp;

	SimpleMath::Vector2 currentTile;

	arr.push_back(Vector3(startPoint.x, startPoint.y, 0));
	allPaths.push_back(arr);
	visited.push_back(SimpleMath::Vector2(startPoint.x, startPoint.y));

	int k = 1;
	int colorIndex = 0;

	while (true)
	{
		for (size_t i = 0; i < allPaths[k - 1].size(); ++i)
		{
			switch (gridType)
			{
			case HEXAGON:
				neighbors = GetNeighborsHexagon(SimpleMath::Vector2(allPaths[k - 1][i].x, allPaths[k - 1][i].y));
				break;
			case SQUARE:
				neighbors = GetNeighborsSquare(SimpleMath::Vector2(allPaths[k - 1][i].x, allPaths[k - 1][i].y));
				break;
			}


			for (size_t ii = 0; ii < neighbors.size(); ++ii)
			{
				if (IsInVector(visited, neighbors[ii]) == false)
				{
					visited.push_back(neighbors[ii]);
					temp.push_back(Vector3(neighbors[ii].x, neighbors[ii].y, k));

				}
				currentTile = neighbors[ii];

				if (currentTile.x == endPoint.x && currentTile.y == endPoint.y)
				{
					break;
				}
			}
			if (currentTile.x == endPoint.x && currentTile.y == endPoint.y)
			{
				break;
			}
		}
		if (currentTile.x == endPoint.x && currentTile.y == endPoint.y)
		{
			break;
		}

		colorIndex++;
		if (colorIndex > 2)
			colorIndex = 0;

		allPaths.push_back(temp);
		k++;
	}
	gridRow[(int)endPoint.x].gridCollum[(int)endPoint.y].tiles.tile->meshRenderer.texture = new Texture(color[4]);
	

	vector<SimpleMath::Vector2> path = CalculatePathSample(allPaths, endPoint);
	std::reverse(path.begin(), path.end());
	return path;
}


vector<SimpleMath::Vector2> PathFinding::GetPathA(SimpleMath::Vector2 startPoint, SimpleMath::Vector2 endPoint, GridType gridType)
{
	//creating variables
	std::map<SimpleMath::Vector2, SimpleMath::Vector2> cameFrom;
	std::map<SimpleMath::Vector2, float> costSoFar;
	std::priority_queue<tilePriorityElement, vector<tilePriorityElement>, CustomCompare> priorityQueue;
	SimpleMath::Vector2 currentPoint;
	vector<SimpleMath::Vector2> path;
	vector<SimpleMath::Vector2> neighbors;
	float newCost;
	float priority;

	//Give startPoint Color
	gridRow[(int)startPoint.x].gridCollum[(int)startPoint.y].tiles.tile->meshRenderer.texture = new Texture(color[2]);

	//Push the startPoint in the list
	priorityQueue.push(std::make_pair(0, startPoint));

	//Push the startPoint in the cameFrom map
	cameFrom[startPoint] = startPoint;
	
	//Push the startpoint in the costSoFar map
	costSoFar[startPoint] = 0;

	//Loop
	while (!priorityQueue.empty())
	{
		//Get the top one of the priorityQueue
		currentPoint = priorityQueue.top().second;

		//Popping the element we are going to use
		priorityQueue.pop();

		//Checking if we reached our endPoint
		if (currentPoint == endPoint)
			break;
		
		//Getting the neighbors of the currenPoint
		switch (gridType)
		{
		case HEXAGON:
			neighbors = GetNeighborsHexagon(SimpleMath::Vector2(currentPoint));
			break;
		case SQUARE:
			neighbors = GetNeighborsSquare(SimpleMath::Vector2(currentPoint));
			break;
		}

		//Looping over the neighbors
		for (size_t i = 0; i < neighbors.size(); ++i)
		{
			//Set the new cost
			newCost = costSoFar[currentPoint] + gridRow[neighbors[i].x].gridCollum[neighbors[i].y].tiles.tileMovementCost; //if we want to add that the tiles cost more to move on do that here.;

			//Check if the neighbor is in the costSoFar list or the newcost is lower then the costSoFar of the neighbor
			if (costSoFar.find(neighbors[i]) == costSoFar.end() || newCost < costSoFar[neighbors[i]])
			{
				//Setting the costSoFar to the new cost
				costSoFar[neighbors[i]] = newCost;

				//Calculate the priority
				SimpleMath::Vector3 endPos = gridRow[endPoint.x].gridCollum[endPoint.y].tiles.tile->transform.position;
				SimpleMath::Vector3 neighborsPos = gridRow[neighbors[i].x].gridCollum[neighbors[i].y].tiles.tile->transform.position;
				priority = newCost + Heuristic(SimpleMath::Vector2(endPos.x, endPos.y), SimpleMath::Vector2(neighborsPos.x, neighborsPos.y)) * 1.000001f;

				//Pushing the priority and the neighbor in the queue
				priorityQueue.push(std::make_pair(priority, neighbors[i]));

				//set the cameFrom the neighbor to the current node
				cameFrom[neighbors[i]] = currentPoint;

				//Give the neighbor we used a color
				//gridRow[(int)neighbors[i].x].gridCollum[(int)neighbors[i].y].tiles.tile->meshRenderer.texture = new Texture(color[3]);
			}
		}
	}

	if (currentPoint != endPoint)
	{
		std::cout << "No Path Found" << endl;
		return vector<SimpleMath::Vector2>();
	}

	//Give the endPoint a color
	gridRow[(int)endPoint.x].gridCollum[(int)endPoint.y].tiles.tile->meshRenderer.texture = new Texture(color[1]);

	//Calulate path
	path = CalculatePathA(cameFrom, endPoint, startPoint);

	//Reverse the array so we have the path from start to end
	std::reverse(path.begin(), path.end());

	return path;
}
#pragma endregion

#pragma region GetNeighbors

vector<SimpleMath::Vector2> PathFinding::GetNeighborsSquare(SimpleMath::Vector2 currenTile)
{
	vector<SimpleMath::Vector2> neibors;

	for (size_t i = 0; i < square.size(); ++i)
	{
		if (CheckTileAtPoint(currenTile + square[i]))
		{
			neibors.push_back(currenTile + square[i]);
		}
	}

	return neibors;
}

vector<SimpleMath::Vector2> PathFinding::GetNeighborsHexagon(SimpleMath::Vector2 currenTile)
{
	vector<SimpleMath::Vector2> neibors;
	
	if ((int)currenTile.y % 2 == 0)
	{
		for (size_t i = 0; i < evenr.size(); ++i)
		{
			if (CheckTileAtPoint(currenTile + evenr[i]))
			{
				neibors.push_back(currenTile + evenr[i]);
			}
		}
	}
	else
	{
		for (size_t i = 0; i < oddr.size(); ++i)
		{
			if (CheckTileAtPoint(currenTile + oddr[i]))
			{
				neibors.push_back(currenTile + oddr[i]);
			}
		}
	}
	return neibors;
}
#pragma endregion


#pragma region CalcluatePath

vector<SimpleMath::Vector2> PathFinding::CalculatePathSample(vector<vector<Vector3>> allPaths, SimpleMath::Vector2 endPoint)
{
	vector<SimpleMath::Vector2> path;
	SimpleMath::Vector2 closestTile;

	path.push_back(endPoint);

	for (int i = allPaths.size(); i > 0; --i)
	{		
		closestTile = GetClosestTile(path[path.size()-1], allPaths[i - 1]);
		path.push_back(closestTile);
		gridRow[(int)closestTile.x].gridCollum[(int)closestTile.y].tiles.tile->meshRenderer.texture = new Texture(color[3]);
	}

	return path;
}

vector<SimpleMath::Vector2> PathFinding::CalculatePathA(std::map<SimpleMath::Vector2, SimpleMath::Vector2> cameFrom, SimpleMath::Vector2 endPoint, SimpleMath::Vector2 startPoint)
{
	vector<SimpleMath::Vector2> path;

	SimpleMath::Vector2 currentPoint = endPoint;
	path.push_back(currentPoint);

	while (currentPoint != startPoint)
	{
		currentPoint = cameFrom[currentPoint];
		gridRow[(int)currentPoint.x].gridCollum[(int)currentPoint.y].tiles.tile->meshRenderer.texture = new Texture(color[4]);
		path.push_back(currentPoint);
	}


	return path;
}
#pragma endregion



SimpleMath::Vector2 PathFinding::GetClosestTile(SimpleMath::Vector2 tile, vector<Vector3> tiles)
{
	float dist;
	float lowestDist = DistanceBetweenTiles(tile, SimpleMath::Vector2(tiles[0].x, tiles[0].y));
	int index = 0;

	for (size_t i = 0; i < tiles.size(); ++i)
	{
		dist = DistanceBetweenTiles(tile, SimpleMath::Vector2(tiles[i].x, tiles[i].y));
		if (dist < lowestDist)
		{
			lowestDist = dist;
			index = i;
		}
	}

	return SimpleMath::Vector2(tiles[index].x, tiles[index].y);
}

bool PathFinding::IsInVector(vector<SimpleMath::Vector2> vector, SimpleMath::Vector2 point)
{
	for (size_t i = 0; i < vector.size(); ++i)
	{
		if (vector[i].x == point.x && vector[i].y == point.y)
		{
			return true;
		}
	}
	return false;
}

bool PathFinding::CheckTileAtPoint(SimpleMath::Vector2 point)
{
	if (point.x < gridRow.size() && point.x >= 0)
	{
		if (point.y < gridRow[(int)point.x].gridCollum.size() && point.y >= 0)
		{
			if (gridRow[(int)point.x].gridCollum[(int)point.y].tiles.tileType != TileType::EmptyTile && gridRow[(int)point.x].gridCollum[(int)point.y].tiles.tileType != TileType::BlockedTile)
			{
				return true;
			}
		}
	}
	return false;
}




float PathFinding::Heuristic(SimpleMath::Vector2 tile1, SimpleMath::Vector2 tile2)
{
	//return DistanceBetweenTiles(SimpleMath::Vector2(tile1.x, tile1.y), SimpleMath::Vector2(tile2.x,tile2.y));
	return std::abs(tile1.x - tile2.x) + std::abs(tile1.y - tile2.y);

	//return std::abs((tile1.x - tile2.x) + (tile1.y - tile2.y));
}

#pragma region DistanceChecks

float PathFinding::DistanceBetweenTiles(SimpleMath::Vector2 tile1, SimpleMath::Vector2 tile2)
{
	return ExMath::dist(tile1.x, tile1.y, tile2.x, tile2.y);
}
#pragma endregion

SimpleMath::Vector2 PathFinding::GetClosestEnemyTile(SimpleMath::Vector2 currentTile, int owner)
{
	float dist;
	float lowestDist = 10000;
	SimpleMath::Vector2 closestEnemyTile;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.tileType == ClaimedTile && 
				gridRow[i].gridCollum[ii].tiles.owner != 0 && 
				gridRow[i].gridCollum[ii].tiles.tileType != owner)
			{
				dist = DistanceBetweenTiles(currentTile, SimpleMath::Vector2(i, ii));

				if (dist < lowestDist)
				{
					closestEnemyTile = SimpleMath::Vector2(i, ii);
					lowestDist = dist;
				}
			}
		}
	}

	return closestEnemyTile;
}

vector<SimpleMath::Vector2> PathFinding::GetAllEnemyTiles(SimpleMath::Vector2 currentTile, int owner)
{
	vector<SimpleMath::Vector2> enemyTiles;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.tileType == ClaimedTile &&
				gridRow[i].gridCollum[ii].tiles.owner != 0 &&
				gridRow[i].gridCollum[ii].tiles.owner != owner)
			{
				enemyTiles.push_back(SimpleMath::Vector2(i, ii));
			}
		}
	}
	return enemyTiles;
}

float PathFinding::DistanceOfPathBetweenTiles(SimpleMath::Vector2 tile1, SimpleMath::Vector2 tile2, GridType gridType)
{
	vector<SimpleMath::Vector2> path = GetPathA(tile1, tile2, gridType);

	return DistanceOfPath(path);
}

float PathFinding::DistanceOfPath(vector<SimpleMath::Vector2> path)
{
	float dist = 0;
	SimpleMath::Vector3 tile1;
	SimpleMath::Vector3 tile2;

	for (size_t i = 1; i < path.size(); ++i)
	{
		tile1 = gridRow[path[i-1].x].gridCollum[path[i - 1].y].tiles.tile->transform.position;
		tile2 = gridRow[path[i].x].gridCollum[path[i].y].tiles.tile->transform.position;

		dist += DistanceBetweenTiles(SimpleMath::Vector2(tile1.x, tile1.y), SimpleMath::Vector2(tile2.x, tile2.y));
	}

	return dist;
}

int PathFinding::GetNumberOfUnits(int owner)
{
	int index = 0;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.owner == owner)
			{
				for (size_t iii = 0; iii < gridRow[i].gridCollum[ii].tiles.unitInfo.size(); ++iii)
				{
					for (size_t iiii = 0; iiii < gridRow[i].gridCollum[ii].tiles.unitInfo[iii].units.size(); ++iiii)
					{
						index++;
					}
				}
			}
		}
	}
	return index;
}

int PathFinding::GetNumberOfUnits(int owner, std::string tag)
{
	int index = 0;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.owner == owner)
			{
				for (size_t iii = 0; iii < gridRow[i].gridCollum[ii].tiles.unitInfo.size(); ++iii)
				{
					for (size_t iiii = 0; iiii < gridRow[i].gridCollum[ii].tiles.unitInfo[iii].units.size(); ++iiii)
					{
						if (gridRow[i].gridCollum[ii].tiles.unitInfo[iii].units[iiii].tag == tag)
						{
							index++;
						}
					}
				}
			}
		}
	}
	return index;
}

vector<SimpleMath::Vector2> PathFinding::GetAllTilesUnitAreOn(int owner)
{
	vector<SimpleMath::Vector2> tiles;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.owner == owner)
			{
				if (gridRow[i].gridCollum[ii].tiles.unitInfo.size() > 0)
				{
					tiles.push_back(SimpleMath::Vector2(i, ii));
				}
			}
		}
	}

	return tiles;
}

vector<SimpleMath::Vector2> PathFinding::GetAllTilesUnitAreOn(int owner, std::string tag)
{
	vector<SimpleMath::Vector2> tiles;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.owner == owner)
			{
				for (size_t iii = 0; iii < gridRow[i].gridCollum[ii].tiles.unitInfo.size(); ++iii)
				{
					if (gridRow[i].gridCollum[ii].tiles.unitInfo[iii].units[0].tag == tag)
					{
						tiles.push_back(SimpleMath::Vector2(i, ii));
						break;
					}
				}
			}
		}
	}

	return tiles;
}

int PathFinding::GetNumberOfTiles(int owner)
{
	int index = 0;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.owner == owner)
			{
				index++;			
			}
		}
	}
	return index;
}

int PathFinding::GetNumberOfTiles(TileType tileType)
{
	int index = 0;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.tileType == tileType)
			{
				index++;
			}
		}
	}
	return index;
}

vector<SimpleMath::Vector2> PathFinding::GetTiles(int owner)
{
	vector<SimpleMath::Vector2> tiles;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.owner == owner)
			{
				tiles.push_back(SimpleMath::Vector2(i, ii));
			}
		}
	}
	return tiles;
}

vector<SimpleMath::Vector2> PathFinding::GetTiles(TileType tileType)
{
	vector<SimpleMath::Vector2> tiles;

	for (size_t i = 0; i < gridRow.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[i].gridCollum.size(); ++ii)
		{
			if (gridRow[i].gridCollum[ii].tiles.tileType == tileType)
			{
				tiles.push_back(SimpleMath::Vector2(i, ii));
			}
		}
	}
	return tiles;
}

int PathFinding::GetNumberOfUnitsOnTile(SimpleMath::Vector2 tile)
{
	int index = 0;

	for (size_t i = 0; i < gridRow[tile.x].gridCollum[tile.y].tiles.unitInfo.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[tile.x].gridCollum[tile.y].tiles.unitInfo[i].units.size(); ++ii)
		{
			index++;	
		}
	}
	return index;
}

int PathFinding::GetNumberOfUnitsOnTile(SimpleMath::Vector2 tile, std::string tag)
{
	int index = 0;

	for (size_t i = 0; i < gridRow[tile.x].gridCollum[tile.y].tiles.unitInfo.size(); ++i)
	{
		for (size_t ii = 0; ii < gridRow[tile.x].gridCollum[tile.y].tiles.unitInfo[i].units.size(); ++ii)
		{
			if (gridRow[tile.x].gridCollum[tile.y].tiles.unitInfo[i].units[ii].tag == tag)
			{
				index++;
			}
		}
	}
	return index;
}
