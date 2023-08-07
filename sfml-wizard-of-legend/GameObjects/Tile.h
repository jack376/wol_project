#pragma once
#include "GameObject.h"

class BaseUI;
class Tile : public GameObject
{
public:
    enum class TileType
    {
        None,
        Ground,
        Cliff,
        Wall,
        MonsterSpawn,
        EventTrigger,
    };
    enum class TileState
    {
        None,
        Select,
        Copy,
        Paste,
        Cut,
    };

protected:
    TileType type;
    TileState state;

    bool isCollision = false;
    bool isHover = false;

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(16.0f, 16.0f));
    sf::Sprite sprite;
    sf::Text text;

    std::map<TileState, sf::Color> stateColor =
    {
        { TileState::None, sf::Color::Transparent },
        { TileState::Select, sf::Color::Cyan },
        { TileState::Copy, sf::Color::Yellow },
        { TileState::Paste, sf::Color::Green },
        { TileState::Cut, sf::Color::Magenta },
    };

public:
    std::function<void()> OnClickDown;
    std::function<void()> OnClickDrag;
    std::function<void()> OnClickUp;
    std::function<void()> OnEnter;
    std::function<void()> OnExit;

    Tile(const std::string& n = "", TileState state = TileState::None);
    virtual ~Tile();

    virtual void Init();
    virtual void Reset();
    virtual void Update(float dt);
    virtual void Draw(sf::RenderWindow& window);

    void SetCollision(bool collision) { isCollision = collision; }

    void SetShapeColor(sf::Color color);
    void SetShapePosition(float x, float y);
    void SetStrokeColor(sf::Color color = sf::Color::Transparent);

    void SetState(TileState state = TileState::None);
    void SetStateColor(TileState state = TileState::None);
    TileState GetState() const;
};

