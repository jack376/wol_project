#include "stdafx.h"
#include "RoofGo.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"
#include "SceneGame.h"

void RoofGo::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr) { sprite.setTexture(*tex); }
}

void RoofGo::Update(float dt)
{
}

void RoofGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void RoofGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
}

void RoofGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
}

void RoofGo::SetOrigin(Origins origin)
{
	const sf::Texture* tex = sprite.getTexture();

	if (!tex)
	{
		std::cerr << "RoofGo SetOrigin Error" << std::endl;
		return;
	}
	sf::Vector2f originPos(sprite.getTexture()->getSize());
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	sprite.setOrigin(originPos);
}

void RoofGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void RoofGo::SetScale(float x, float y)
{
	sprite.setScale(x, y);
}

void RoofGo::SetTexture(const std::string& id)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
	textureId = id;
}

void RoofGo::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	sprite.setTextureRect(rect);
}