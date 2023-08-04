#pragma once
#include "GameObject.h"

class Tile : public GameObject
{
public:
    enum class TileType
    {
        None = -1,
        Ground,
        Cliff,
        Wall,
        MonsterSpawn,
        EventTrigger,
    };

private:
    TileType type;
    bool isCollision = false;
    bool isHover = false;

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(64.0f, 64.0f));
    sf::Sprite sprite;
    sf::Text text;
    sf::Color color;

public:
    std::function<void()> OnClick;
    std::function<void()> OnEnter;
    std::function<void()> OnExit;

    Tile(const std::string& n = "", TileType type = TileType::None);
    virtual ~Tile();

    void SetCollision(bool collision) { isCollision = collision; }
    void SetShapeColor(sf::Color color = sf::Color::Transparent);
    void SetStrokeColor(sf::Color color = sf::Color::Transparent);
    void SetShapePosition(float x, float y);

    virtual void Init();
    virtual void Reset();
    virtual void Update(float dt);
    virtual void Draw(sf::RenderWindow& window);
};

