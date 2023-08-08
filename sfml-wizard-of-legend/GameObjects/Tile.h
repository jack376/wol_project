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
        Blank,
        Select,
        Copy,
        Paste,
        Cut,
        UI,
        SelectUI,
    };

protected:
    TileType type;
    TileState state;

    bool isCollision = false;
    bool isHover = false;
    
    const float tileSize = 16.0f;
    const int textureAtlasSize = 512;

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));
    sf::Sprite sprite;
    sf::Text text;

    std::map<TileState, sf::Color> stateColor =
    {
        { TileState::Blank,    sf::Color::Color(0, 0, 0, 0)       },
        { TileState::Select,   sf::Color::Color(0, 128, 0, 128)   },
        { TileState::Copy,     sf::Color::Color(192, 128, 0, 128) },
        { TileState::Paste,    sf::Color::Color(128, 0, 0, 128)   },
        { TileState::Cut,      sf::Color::Color(0, 128, 128, 128) },
        { TileState::UI,       sf::Color::Color(0, 0, 0, 0)       },
        { TileState::SelectUI, sf::Color::Color(0, 128, 192, 128) },
    };

public:
    std::function<void()> OnClickDown;
    std::function<void()> OnClickDrag;
    std::function<void()> OnClickUp;
    std::function<void()> OnEnter;
    std::function<void()> OnExit;

    Tile(const std::string& n = "", TileState state = TileState::Blank);
    virtual ~Tile();

    virtual void Init();
    virtual void Reset();
    virtual void Update(float dt);
    virtual void Draw(sf::RenderWindow& window);

    void SetCollision(bool collision) { isCollision = collision; }

    void SetShapeColor(sf::Color color);
    void SetShapePosition(float x, float y);
    void SetStrokeColor(sf::Color color = sf::Color::Transparent);
    void SetSpritePosition(float x, float y);

    void SetState(TileState state = TileState::Blank);
    void SetStateColor(TileState state = TileState::Blank);

    TileState GetState() const;

    void SetTexture(const sf::Texture& tex);
    void SetTextureRect(const sf::IntRect& rect);

    sf::IntRect GetTextureRect(int tileIndex = 0) const;
    sf::Vector2f GetMousePosBasedOnState() const;
};

