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
    int indexLeft = 31;
    int indexTop = 0;

    SetTexture(*RESOURCE_MGR.GetTexture("graphics/editor/FireTileSet.png"));
    SetTextureRect(sf::IntRect(indexLeft * tileSize, indexTop * tileSize, tileSize, tileSize), "graphics/editor/FireTileSet.png");
    shape.setFillColor(sf::Color::Transparent);

    if (SCENE_MGR.GetCurrSceneId() == SceneId::Editor)
    {
        sprite.setColor(sf::Color::Transparent);
        shape.setOutlineThickness(1.0f);
    }
}

void Tile::Update(float dt)
{
    sf::Vector2f mousePos = GetMousePosBasedOnState();

    bool prevHover = isHover;
    isHover = shape.getGlobalBounds().contains(mousePos);

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
}

void Tile::Draw(sf::RenderWindow& window)
{
    window.draw(sprite);
    window.draw(shape);
    //window.draw(text);
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
    sf::Vector2f originPos(sprite.getTexture()->getSize());
    originPos.x *= ((int)origin % 3) * 0.5f;
    originPos.y *= ((int)origin / 3) * 0.5f;
    sprite.setOrigin(originPos);
}

void Tile::SetOrigin(float x, float y)
{
    sprite.setOrigin(x, y);
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
    sprite.setPosition(x, y);
}

void Tile::SetScale(float scale)
{
    sprite.setScale(scale, scale);
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

Tile::TileType Tile::GetType() const
{
    return type;
}

void Tile::SetLayer(int tileLayer)
{
    this->tileLayer = tileLayer;
}

int Tile::GetLayer() const
{
    return tileLayer;
}

void Tile::SetState(TileState state)
{
    this->state = state;
}

void Tile::SetStateColor(TileState state)
{
    shape.setFillColor(stateColor[state]);
}

Tile::TileState Tile::GetState() const
{
    return state;
}

void Tile::SetTexture(const sf::Texture& tex)
{
    sprite.setTexture(tex);
}

void Tile::SetTextureRect(const sf::IntRect& rect, const std::string& path)
{
    sprite.setTexture(*RESOURCE_MGR.GetTexture(path));
    sprite.setTextureRect(rect);
    sprite.setColor(sf::Color::White);
}

sf::IntRect Tile::GetTextureRect() const
{
    return sprite.getTextureRect();
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