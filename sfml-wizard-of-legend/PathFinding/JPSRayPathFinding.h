#pragma once
#include "stdafx.h"
#include "Singleton.h"
#include "Beam.h"

class Tile;

struct Node
{
	Tile* nodeTile;
	sf::Vector2f position;	//Tile의 중심위치

	double caculatorCost;		
	double pathCost;
};

class JPSRayPathFinding : public Singleton<JPSRayPathFinding>
{
	friend Singleton<JPSRayPathFinding>;
protected:
	JPSRayPathFinding() = default;
	virtual ~JPSRayPathFinding() override = default;

	Beam raycaster;
	std::vector<Node> pathNode;
	std::vector<std::vector<Tile*>>* tilesWorld = nullptr;

	double heuristicEstimate;	//예상최단경로
	double totalCaculatorCost;
	double totalPathCost;

public:
	
	std::pair<bool, Tile*> CheckShortestPath(Tile* start, Tile* goal, const std::vector<Tile*> tiles, const std::vector<std::vector<Tile*>>* tilesWorld);
	std::pair<Tile*, int> StraightCheck(Tile* tile, TileType type, sf::Vector2i dir);
	std::vector<Node> FindCorner(Tile* wall);
	std::vector<Node> FindPath(Tile* start, Tile* goal, const std::vector<Tile*> tiles, const std::vector<std::vector<Tile*>>* tilesWorld);
	

	void SetTiles(std::vector<std::vector<Tile*>>* tiles);
};

#define JPSRAYPATHFINDING (JPSRayPathFinding::Instance())

