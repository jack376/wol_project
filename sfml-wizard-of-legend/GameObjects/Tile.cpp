#include "stdafx.h"
#include "Tile.h"
#include "BaseUI.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "Framework.h"

Tile::Tile(const std::string& n, TileState state, TileType type) : GameObject(n)
{
}

Tile::~Tile()
{
    Release();
}

void Tile::Init()
{
}

void Tile::Reset()
{
    if (SCENE_MGR.GetCurrSceneId() == SceneId::Editor)
    {
        shape.setOutlineThickness(1.0f);
    }
}

void Tile::Update(float dt)
{
    sf::Vector2f mousePos = GetMousePosBasedOnState();

    bool prevHover = isHover;
    isHover = shape.getGlobalBounds().contains(mousePos);

    // Button Action
    if (!prevHover && isHover)
    {
        if (OnEnter != nullptr)
        {
            OnEnter();
        }
    }
    if (prevHover && !isHover)
    {
        if (OnExit != nullptr)
        {
            OnExit();
        }
    }
    if (isHover)
    {
        if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left))
        {
            if (OnClickDown != nullptr)
            {
                OnClickDown();
            }
        }
        else if (INPUT_MGR.GetMouseButton(sf::Mouse::Left))
        {
            if (OnClickDrag != nullptr)
            {
                OnClickDrag();
            }
        }
        else if (INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left))
        {
            if (OnClickUp != nullptr)
            {
                OnClickUp();
            }
        }
    }

    // TileType View Mode
    if (!isTypeView && INPUT_MGR.GetKeyDown(sf::Keyboard::T))
    {
        isTypeView = !isTypeView;
    }
    else if (isTypeView && INPUT_MGR.GetKeyDown(sf::Keyboard::T))
    {
        shape.setFillColor(sf::Color::Transparent);
        isTypeView = !isTypeView;
    }
    if (isTypeView && !INPUT_MGR.GetMouseButton(sf::Mouse::Left))
    {
        SetTypeColor(GetType());
    }
}

void Tile::Draw(sf::RenderWindow& window)
{
    window.draw(spriteBottom);
    window.draw(spriteTop);
    window.draw(shape, sf::BlendAlpha);
}

void Tile::SetCollision(bool collision)
{
    isCollision = collision;
}

bool Tile::GetCollision()
{
    return isCollision;
}

void Tile::SetOrigin(Origins origin)
{
    sf::Vector2f originPos(spriteTop.getTexture()->getSize());
    originPos.x *= ((int)origin % 3) * 0.5f;
    originPos.y *= ((int)origin / 3) * 0.5f;
    spriteTop.setOrigin(originPos);
}

void Tile::SetOrigin(float x, float y)
{
    spriteTop.setOrigin(x, y);
}

void Tile::SetTileSize(int tileSize)
{
    this->tileSize = tileSize;
}

void Tile::SetShapeColor(sf::Color color)
{
    shape.setFillColor(sf::Color(color));
}

void Tile::SetStrokeColor(sf::Color color)
{
    shape.setOutlineColor(sf::Color(color));
}

void Tile::SetShapePosition(float x, float y)
{
    shape.setPosition(x, y);
}

void Tile::SetSpritePosition(float x, float y)
{
    spriteTop.setPosition(x, y);
}

void Tile::SetScale(float scale)
{
    spriteTop.setScale(scale, scale);
}

void Tile::SetIndex(int x, int y)
{
    tileIndexX = x;
    tileIndexY = y;
}

sf::Vector2i Tile::GetIndex() const
{
    return sf::Vector2i(tileIndexX, tileIndexY);
}

void Tile::SetType(TileType type)
{
    this->type = type;
}

void Tile::SetLayer(int tileLayer)
{
    this->tileLayer = tileLayer;
}

int Tile::GetLayer() const
{
    return tileLayer;
}

void Tile::SetTypeColor(TileType type)
{
    std::map<TileType, sf::Color> typeColor =
    {
        { TileType::None,         sf::Color::Transparent      },
        { TileType::Ground,       sf::Color(0, 192, 0, 128)   },
        { TileType::Cliff,        sf::Color(255, 0, 0, 128)   },
        { TileType::Wall,         sf::Color(255, 160, 0, 128) },
        { TileType::MonsterSpawn, sf::Color(128, 0, 255, 128) },
        { TileType::EventTrigger, sf::Color(0, 0, 255, 128)   },
    };
    shape.setFillColor(typeColor[type]);
}

Tile::TileType Tile::GetType() const
{
    return type;
}

void Tile::SetTypeView(bool typeView)
{
    isTypeView = typeView;
}

bool Tile::IsTypeView() const
{
    return isTypeView;
}

void Tile::SetState(TileState state)
{
    this->state = state;
}

void Tile::SetStateColor(TileState state)
{
    std::map<TileState, sf::Color> stateColor =
    {
        { TileState::Blank,    sf::Color::Transparent     },
        { TileState::Select,   sf::Color(0, 0, 128, 64)   },
        { TileState::Copy,     sf::Color(192, 128, 0, 64) },
        { TileState::Paste,    sf::Color(128, 0, 0, 64)   },
        { TileState::Cut,      sf::Color(0, 128, 128, 64) },
        { TileState::UI,       sf::Color::Transparent     },
        { TileState::SelectUI, sf::Color(0, 128, 192, 64) },
    };
    shape.setFillColor(stateColor[state]);
}

Tile::TileState Tile::GetState() const
{
    return state;
}

void Tile::SetTexture(const sf::Texture& tex)
{
    spriteTop.setTexture(tex);
}

void Tile::SetTextureRect(const sf::IntRect& rect, const std::string& path)
{
    spriteTop.setTexture(*RESOURCE_MGR.GetTexture(path));
    spriteTop.setTextureRect(rect);
    spriteTop.setColor(sf::Color::White);
}

sf::IntRect Tile::GetTextureRect() const
{
    return spriteTop.getTextureRect();
}

sf::Vector2f Tile::GetMousePosBasedOnState() const
{
    sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
    switch (state)
    {
    case TileState::Blank:
        return SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);
    case TileState::UI:
        return SCENE_MGR.GetCurrScene()->ScreenToUiPos(mousePos);
    default:
        return mousePos;
    }
}