#include "stdafx.h"
#include "MenuInventory.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"

MenuInventory::MenuInventory(const std::string& n) : GameObject(n)
{
}

MenuInventory::~MenuInventory()
{
}

void MenuInventory::SetPosition(const sf::Vector2f& p)
{
	position = p;
	panel.setPosition(p);
}

void MenuInventory::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	panel.setPosition({ x,y });
}

void MenuInventory::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(panel, origin);
	}
}

void MenuInventory::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	panel.setOrigin(x, y);
}

void MenuInventory::SetOrigin(sf::RectangleShape& rect, Origins origin)
{
	GameObject::SetOrigin(origin);
	//if (this->origin != Origins::CUSTOM)
	//{
	//	Utils::SetOrigin(rect, origin);
	//}
}

void MenuInventory::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();

	
	panel.setTexture(*RESOURCE_MGR.GetTexture("graphics/UI/Panel.png"));
	SetPosition(windowSize.x * 0.3f,windowSize.y * 0.5f);
	SetOrigin(Origins::MC);
	panel.setScale(6, 8);


	sortLayer = 101;
}

void MenuInventory::Release()
{
}

void MenuInventory::Reset()
{
}

void MenuInventory::Update(float dt)
{
}

void MenuInventory::Draw(sf::RenderWindow& window)
{
	window.draw(panel);
}
