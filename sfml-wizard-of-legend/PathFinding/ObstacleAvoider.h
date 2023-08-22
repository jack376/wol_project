#pragma once
#include "stdafx.h"
#include "Singleton.h"
#include "Beam.h"

class Tile;

struct Node
{
	Tile* nodeTile;
	sf::Vector2f position;	//Tile의 중심위치
};

class ObstacleAvoider : public Singleton<ObstacleAvoider>
{
	friend Singleton<ObstacleAvoider>;
protected:
	ObstacleAvoider() = default;
	virtual ~ObstacleAvoider() override = default;

	Beam raycaster;
	std::vector<Node> pathNode;
	std::vector<std::vector<Tile*>>* tilesWorld = nullptr;
	std::vector<Tile*>* nonGroundTile = nullptr;

public:
	
	std::pair<bool, Tile*> CheckShortestPath(Tile* start, Tile* goal);
	Tile* StraightCheck(Tile* tile, TileType type, sf::Vector2i dir);	//type == ground
	std::pair<bool, std::vector<Tile*>> CheckCorner(Tile* tile);
	Node TileToNode(Tile* tile);
	std::vector<Node> FindPath(Tile* start, Tile* goal);	

	void SetTiles(std::vector<std::vector<Tile*>>* tiles);
};

#define OBSTACLEAVOIDER (ObstacleAvoider::Instance())

