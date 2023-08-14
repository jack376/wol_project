#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Line.h"

class Tile;
class Player;

struct Wall
{
    sf::Vector2f p1;
    sf::Vector2f p2;
};

class Beam
{
private:
    Line m_line;
    int m_x, m_y;
    float m_angle;
public:
    Beam(int x, int y, float angle);
    ~Beam();
    void draw(sf::RenderWindow &window) const;
    void move(int x, int y);
    void checkCollision(const Tile& tile);
    bool CheckTile(const Tile& tile);
     CalculateWall(const Tile& tile);
};
