#include "stdafx.h"
#include "JPSRayPathFinding.h"
#include "Tile.h"

#define _Left sf::Vector2i(-1, 0)
#define _Right sf::Vector2i(1, 0)
#define _Up sf::Vector2i(0, -1)
#define _Down sf::Vector2i(0, 1)


std::pair<bool, Tile*> JPSRayPathFinding::CheckShortestPath(Tile* start, Tile* goal, const std::vector<Tile*> tiles, const std::vector<std::vector<Tile*>>* tilesWorld)
{
    return raycaster.CheckShortestPath(start->GetPosition(), goal->GetPosition(), tiles, tilesWorld);
}

std::pair<Tile*, int> JPSRayPathFinding::StraightCheck(Tile* tile, TileType type, sf::Vector2i dir)
{
    int cost = 1;
    sf::Vector2i index = tile->GetIndex();
    
    while ((*tilesWorld)[index.x + dir.x * cost][index.y + dir.y * cost]->GetType() == type)
    {
        index = { index.x + dir.x * cost, index.y + dir.y * cost };
        cost++;
    }

    return std::pair<Tile*, int>((*tilesWorld)[index.x][index.y], cost - 1);
}


std::vector<Node> JPSRayPathFinding::FindCorner(Tile* wallTile)
{
    sf::Vector2i index = wallTile->GetIndex();





    return std::vector<Node>();
}


std::vector<Node> JPSRayPathFinding::FindPath(Tile* start, Tile* goal, const std::vector<Tile*> tiles, const std::vector<std::vector<Tile*>>* tilesWorld)
{
    std::pair<bool, Tile*> result = CheckShortestPath(start, goal, tiles, tilesWorld);
    std::vector<Node> nodes;

    if (result.first)
    {
        Node node =
        {
            result.second,
            result.second->GetPosition() + sf::Vector2f(_TileSize, _TileSize),
            1,
            1
        };
        nodes.push_back(node);
        return nodes;
    }
    else
    {
        FindCorner(result.second);
        
    }
}


void JPSRayPathFinding::SetTiles(std::vector<std::vector<Tile*>>* tiles)
{
    tilesWorld = tiles;
}
