#pragma once
#include "Line.h"

class Tile;
class Player;
class Monster;

struct Wall
{
    sf::Vector2i p1;
    sf::Vector2i p2;
};

class Beam
{
private:
    Line m_line;
    int m_x, m_y;
    float m_angle;

public:
    Beam(int x = 0, int y = 0, float angle = 0);
    ~Beam();
    void draw(sf::RenderWindow &window) const;
    void move(int x, int y);
    void Rotation(float angle);

    const std::pair<bool, Tile*> CheckShortestPath(const sf::Vector2f start, const sf::Vector2f goal, const std::vector<Tile*> tiles, const std::vector<std::vector<Tile*>>* worldTiles);  //tiles == Wall, Cliffs

    void checkCollision(const std::vector<Tile*> tiles, Player* player);
    void checkCollision(Monster* monster);
    sf::Vector2f GetEndPos() { return  m_line.getEndPoint(); }

    const sf::Vector2i PosToIndex(sf::Vector2f pos);
};
