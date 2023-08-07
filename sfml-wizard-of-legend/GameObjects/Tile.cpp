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
    sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/editor/Temp.png"));
    sprite.setOrigin(0.0f, 0.0f);
    shape.setOutlineThickness(0.5f);
}

void Tile::Update(float dt)
{
    sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
    sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

    bool prevHover = isHover;
    isHover = shape.getGlobalBounds().contains(worldMousePos);

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
    //window.draw(sprite);
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
