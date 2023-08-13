#include "stdafx.h"
#include "DestructibleGo.h"
#include "Utils.h"
#include "ResourceMgr.h"

void DestructibleGo::Init()
{
}

void DestructibleGo::Release()
{
}

void DestructibleGo::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr)
	{
		sprite.setTexture(*tex);
	}
	SetOrigin(origin);
	SetPosition(GetPosition());
}

void DestructibleGo::Update(float dt)
{
}

void DestructibleGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void DestructibleGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
}

void DestructibleGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
}

void DestructibleGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(sprite, origin);
	}
}

void DestructibleGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void DestructibleGo::SetFlipX(bool flip)
{
	sprite.setScale((flip) ? -abs(sprite.getScale().x) : abs(sprite.getScale().x), sprite.getScale().y);
}

void DestructibleGo::SetFlipY(bool flip)
{
	sf::Vector2f scale = sprite.getScale();
	scale.y = abs(scale.y);
	scale.y = (flip) ? scale.y : -scale.y;
	sprite.setScale(scale);
}