#include "stdafx.h"
#include "SpriteGo.h"
#include "Utils.h"
#include "ResourceMgr.h"

SpriteGo::SpriteGo(const std::string& textureId, const std::string& n)
	: GameObject(n), textureId(textureId)
{
}

SpriteGo::~SpriteGo()
{
}

void SpriteGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
	rect.setPosition(p);
	animSprite1.setPosition(p);
	animSprite2.setPosition(p);
}

void SpriteGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
	rect.setPosition(position);
	animSprite1.setPosition(position);
	animSprite2.setPosition(position);
}

void SpriteGo::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(sprite, origin);
		Utils::SetOrigin(rect, origin);
		Utils::SetOrigin(animSprite1, origin);
		Utils::SetOrigin(animSprite2, origin);
	}
}

void SpriteGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
	rect.setOrigin(x, y);
	animSprite1.setOrigin(x, y);
	animSprite2.setOrigin(x, y);
}

void SpriteGo::SetOrigin(sf::RectangleShape& rect, Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(rect, origin);
	}
}

void SpriteGo::Init()
{

}

void SpriteGo::Release()
{
}

void SpriteGo::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr)
	{
		sprite.setTexture(*tex);
	}
	SetOrigin(origin);
	SetPosition(GetPosition());
}

void SpriteGo::Update(float dt)
{
}

void SpriteGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(animSprite1);
	window.draw(animSprite2);
	window.draw(rect);
}

void SpriteGo::SetFlipX(bool flip)
{
	sprite.setScale((flip) ? -abs(sprite.getScale().x) : abs(sprite.getScale().x), sprite.getScale().y);
}

void SpriteGo::SetFlipY(bool flip)
{
	sf::Vector2f scale = sprite.getScale();
	scale.y = abs(scale.y);
	scale.y = (flip) ? scale.y : -scale.y;
	sprite.setScale(scale);
}
