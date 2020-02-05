#pragma once
#include <Entity.h>

using namespace std;
using namespace DirectX;

enum TileType
{
	EmptyTile, 
	UnClaimedTile,  
	ClaimedTile, 
	BlockedTile
};

struct UnitInfo
{
	vector<Entity> units;
	SimpleMath::Vector3 possition;
};

struct TileInfo
{
	TileType tileType;
	int tileHeight;
	Entity* tile;
	vector<UnitInfo> unitInfo;
	float tileMovementCost = 1;
	int owner = 0;
};

struct GridCollum
{
	TileInfo tiles;
};

struct GridRow
{
	vector<GridCollum> gridCollum;
};
