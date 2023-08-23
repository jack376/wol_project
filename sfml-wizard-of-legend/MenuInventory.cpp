#include "stdafx.h"
#include "MenuInventory.h"

MenuInventory::MenuInventory(const std::string& n) : GameObject(n)
{
}

MenuInventory::~MenuInventory()
{
}

void MenuInventory::SetPosition(const sf::Vector2f& p)
{
	position = p;
	
}

void MenuInventory::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;

}

void MenuInventory::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	//if (this->origin != Origins::CUSTOM)
	//{
	//	Utils::SetOrigin(sprite, origin);
	//}
}

void MenuInventory::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	//sprite.setOrigin(x, y);
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
	//window.draw();
}
