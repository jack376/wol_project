#pragma once
#include "GameObject.h"

class BaseUI;
class Tile : public GameObject
{
public:
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

    int tileLayer = 0;
    float tileSize = 16.0f;

    bool isCollision = false;
    bool isHover = false;
    bool isTypeView = false;
    bool isGridView = false;

    int tileIndexX = 0;
    int tileIndexY = 0;

    sf::RectangleShape shape = sf::RectangleShape(sf::Vector2f(tileSize, tileSize));

    sf::Sprite spriteTop;
    sf::Sprite spriteBottom;

public:
    std::function<void()> OnClickDown;
    std::function<void()> OnClickDrag;
    std::function<void()> OnClickUp;
    std::function<void()> OnEnter;
    std::function<void()> OnExit;

    Tile(const std::string& n = "", TileState state = TileState::Blank, TileType type = TileType::None);
    virtual ~Tile();

    virtual void Init() {}

    virtual void Reset();
    virtual void Update(float dt);
    virtual void Draw(sf::RenderWindow& window);

    void SetCollision(bool collision);
    bool GetCollision();

    void SetOrigin(Origins origin);
    void SetOrigin(float x, float y);
    void SetShapeColor(sf::Color color);
    void SetStrokeColor(sf::Color color = sf::Color::Transparent);
    void SetScale(float scale = 1.0f);

    void SetPosition(float x, float y);
    sf::Vector2f GetPosition() const;

    void SetTileSize(int tileSize);
    int GetTileSize() const;

    void SetIndex(int x, int y);
    sf::Vector2i GetIndex() const;

    void SetTypeView(bool typeView);
    bool IsTypeView() const;

    void SetGridView(bool gridView);
    bool IsGridView() const;

    void SetLayer(int tileLayer = 0);
    int GetLayer() const;

    void SetType(TileType type = TileType::None);
    void SetTypeColor(TileType type);
    TileType GetType() const;

    void SetState(TileState state = TileState::Blank);
    void SetStateColor(TileState state = TileState::Blank);
    TileState GetState() const;

    void SetTexture(const std::string& path);
    void SetTextureRectTop(const sf::IntRect& rect, const std::string& path);
    sf::IntRect GetTextureRectTop() const;
    void SetTextureRectBottom(const sf::IntRect& rect, const std::string& path);
    sf::IntRect GetTextureRectBottom() const;

    sf::Vector2f GetMousePosBasedOnState() const;

    const sf::FloatRect GetTileGlobalBounds() { return shape.getGlobalBounds(); }
};

