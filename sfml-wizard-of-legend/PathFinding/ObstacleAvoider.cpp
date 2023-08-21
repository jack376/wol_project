#include "stdafx.h"
#include "ObstacleAvoider.h"
#include "Tile.h"

#define _Left sf::Vector2i(-1, 0)
#define _Right sf::Vector2i(1, 0)
#define _Up sf::Vector2i(0, -1)
#define _Down sf::Vector2i(0, 1)
#define _LeftUp sf::Vector2i(-1, -1)
#define _RightUp sf::Vector2i(1, -1)
#define _LeftDown sf::Vector2i(-1, 1)
#define _RightDown sf::Vector2i(1, 1)


std::pair<bool, Tile*> ObstacleAvoider::CheckShortestPath(Tile* start, Tile* goal)
{
    return raycaster.CheckShortestPath(start->GetPosition(), goal->GetPosition(), *nonGroundTile, tilesWorld);
}

Tile* ObstacleAvoider::StraightCheck(Tile* tile, TileType type, sf::Vector2i dir)
{
    int cost = 1;
    sf::Vector2i index = tile->GetIndex();
    
    while ((*tilesWorld)[index.x + dir.x * cost][index.y + dir.y * cost]->GetType() != type)
    {
        index = { index.x + dir.x * cost, index.y + dir.y * cost };
        cost++;
    }
    return (*tilesWorld)[index.x][index.y];
}

std::pair<bool, std::vector<Tile*>> ObstacleAvoider::CheckCorner(Tile* tile)
{
    std::vector<Tile*> corners;
    sf::Vector2i index = tile->GetIndex();
    sf::Vector2i leftUp = index + _LeftUp;
    sf::Vector2i rightUp = index + _RightUp;
    sf::Vector2i leftDown = index + _LeftDown;
    sf::Vector2i rightDown = index + _RightDown;

    if ((*tilesWorld)[leftUp.x][leftUp.y]->GetType() == TileType::Ground)
    {
        sf::Vector2i r = leftUp + _Right;
        sf::Vector2i d = leftUp + _Down;

        if ((*tilesWorld)[r.x][r.y]->GetType() == TileType::Ground &&
            (*tilesWorld)[d.x][d.y]->GetType() == TileType::Ground)
        {
            corners.push_back((*tilesWorld)[leftUp.x][leftUp.y]);
        }
    }

    if ((*tilesWorld)[rightUp.x][rightUp.y]->GetType() == TileType::Ground)
    {
        sf::Vector2i l = rightUp + _Left;
        sf::Vector2i d = rightUp + _Down;

        if ((*tilesWorld)[l.x][l.y]->GetType() == TileType::Ground &&
            (*tilesWorld)[d.x][d.y]->GetType() == TileType::Ground)
        {
            corners.push_back((*tilesWorld)[rightUp.x][rightUp.y]);
        }
    }

    if ((*tilesWorld)[leftDown.x][leftDown.y]->GetType() == TileType::Ground)
    {
        sf::Vector2i r = leftDown + _Right;
        sf::Vector2i u = leftDown + _Up;

        if ((*tilesWorld)[r.x][r.y]->GetType() == TileType::Ground &&
            (*tilesWorld)[u.x][u.y]->GetType() == TileType::Ground)
        {
            corners.push_back((*tilesWorld)[leftDown.x][leftDown.y]);
        }
    }

    if ((*tilesWorld)[rightDown.x][rightDown.y]->GetType() == TileType::Ground)
    {
        sf::Vector2i l = rightDown + _Left;
        sf::Vector2i u = rightDown + _Up;

        if ((*tilesWorld)[l.x][l.y]->GetType() == TileType::Ground &&
            (*tilesWorld)[u.x][u.y]->GetType() == TileType::Ground)
        {
            corners.push_back((*tilesWorld)[rightDown.x][rightDown.y]);
        }
    }
    
    return std::pair<bool, std::vector<Tile*>>(corners.size() != 0, corners);
}


Node ObstacleAvoider::TileToNode(Tile* tile)
{
    Node node =
    {
        tile,
        {tile->GetPosition().x + _TileSize / 2, tile->GetPosition().y + _TileSize / 2}
    };
    return node;
}


std::vector<Node> ObstacleAvoider::FindPath(Tile* start, Tile* goal)
{
    std::pair<bool, Tile*> result = CheckShortestPath(start, goal);
    std::vector<Node> nodes;

    if (result.first)
    {
        Node node = TileToNode(result.second);
        nodes.push_back(node);
        return nodes;
    }
    else
    {
       
        
        
    }
}


void ObstacleAvoider::SetTiles(std::vector<std::vector<Tile*>>* tiles)
{
    tilesWorld = tiles;
}
