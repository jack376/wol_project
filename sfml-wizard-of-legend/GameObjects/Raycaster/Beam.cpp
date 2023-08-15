#include "stdafx.h"
#include "Beam.h"
#include "Tile.h"
#include "Player.h"

#define SX 1280
#define SY 720
#define RAD 3.14159265 / 180.0

Beam::Beam(int x, int y, float angle) : m_x(x), m_y(y), m_angle(angle), m_line(x, y, angle, 2)
{
}

void Beam::draw(sf::RenderWindow &window) const
{
    m_line.draw(window);
}

void Beam::checkCollision(const std::vector<std::vector<Tile*>>& tiles, Player* player)
{
    std::vector<sf::Vector2f> collisionPoints;
    std::vector<Tile*> wallTiles;
    std::vector<Wall> walls;

    //Wall 타일만 추출
    for (auto& tileArray : tiles)
    {
        for (auto& tile : tileArray)
        {
            if (tile->GetType() != Tile::TileType::Wall)
                continue;
            else
                wallTiles.push_back(tile);
        }
    }


    float x1 = m_line.getX1();
    float y1 = m_line.getY1();
    float x2 = m_line.getX2();
    float y2 = m_line.getY2();
    float hypo = sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));

    //타일을 Wall로 분해
    for (auto& tile : wallTiles)
    {
        sf::IntRect rect = tile->GetTextureRectBottom();
        sf::Vector2i p1 = { rect.left, rect.top };
        sf::Vector2i p2 = { rect.left + rect.width, rect.top };
        sf::Vector2i p3 = { rect.left, rect.top + rect.height };
        sf::Vector2i p4 = { rect.left + rect.width, rect.top + rect.height };

        Wall topWall = { p1, p2 };
        Wall leftWall = { p1, p3 };
        Wall rightWall = { p2, p4 };
        Wall bottomWall = { p3, p4 };

        walls.push_back(topWall);
        walls.push_back(leftWall);
        walls.push_back(rightWall);
        walls.push_back(bottomWall);
    }

    {
        sf::IntRect rect = (sf::IntRect)player->sprite.getGlobalBounds();
        sf::Vector2i p1 = { rect.left, rect.top };
        sf::Vector2i p2 = { rect.left + rect.width, rect.top };
        sf::Vector2i p3 = { rect.left, rect.top + rect.height };
        sf::Vector2i p4 = { rect.left + rect.width, rect.top + rect.height };

        Wall topWall = { p1, p2 };
        Wall leftWall = { p1, p3 };
        Wall rightWall = { p2, p4 };
        Wall bottomWall = { p3, p4 };

        walls.push_back(topWall);
        walls.push_back(leftWall);
        walls.push_back(rightWall);
        walls.push_back(bottomWall);
    }



    for (auto wall : walls )
    {
        float x3, y3, x4, y4, numerator1, numerator2, denominator;

        x3 = wall.p1.x;
        y3 = wall.p1.y;
        x4 = wall.p2.x;
        y4 = wall.p2.y;

        numerator1 = (x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4);
        numerator2 = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3));
        denominator = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

        if (denominator != 0)
        {
            const float t = numerator1 / denominator;
            const float u = numerator2 / denominator;
            if (0 < u && 0 < t && u < 1)
            {
                //std::cout << t << " " << u << std::endl;
                float intersection_x = x1 + t * (x2 - x1);
                float intersection_y = y1 + t * (y2 - y1);
                //std::cout << intersection_x << " " << intersection_y << std::endl;
                //m_line.setLength(sqrt(pow(m_x - intersection_x, 2) + pow(m_y - intersection_y, 2)));

                collisionPoints.push_back({ intersection_x, intersection_y });
                //m_line.setEndPoint(intersection_x, intersection_y);
            }
        }
        //    else
        //    {
        //        m_line.setLength(sqrt(pow(SX, 2) + pow(SY, 2)));
        //    }
        //}
        //else
        //{
        //    m_line.setLength(sqrt(pow(SX, 2) + pow(SY, 2)));
        //}
    }

    float distance = m_line.GetLength();
    for (auto& point : collisionPoints)
    {
        float newDistance = Utils::Distance({ (float)m_line.getX1(), (float)m_line.getY1() }, (sf::Vector2f)point);
        if (distance > newDistance)
        {
            distance = newDistance;
            m_line.setEndPoint(point.x, point.y);
        }
       
    }
}

void Beam::move(int x, int y)
{
    m_x = x;
    m_y = y;
    m_line.move(x, y);
}

void Beam::Rotation(float angle)
{
    m_angle = angle;
    m_line.Rotation(m_angle);
}


Beam::~Beam()
{
}