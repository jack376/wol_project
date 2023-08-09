#include "stdafx.h"
#include "Tile.h"
#include "BaseUI.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "ResourceMgr.h"
#include "Framework.h"

Tile::Tile(const std::string& n, TileState type) : GameObject(n)
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
    sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/editor/FireTileSet.png"));
    sprite.setTextureRect(GetTextureRect(31));
    shape.setOutlineThickness(1.0f);
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

void Tile::SetTextureRect(const sf::IntRect& rect)
{
    sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/editor/FireTileSet.png"));
    sprite.setTextureRect(rect);
}

sf::IntRect Tile::GetTextureRect(int tileIndex) const
{
    int textureRectSize = tileSize * 0.5f;
    int tilesPerRow = textureAtlasSize / textureRectSize;

    int x = (tileIndex % tilesPerRow) * tileSize;
    int y = (tileIndex / tilesPerRow) * tileSize;

    return sf::IntRect(x, y, tileSize, tileSize);
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