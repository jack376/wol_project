#include "stdafx.h"
#include "GlowGo.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"
#include "SceneGame.h"

void GlowGo::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr) { sprite.setTexture(*tex); }

	SetOrigin(origin);
	SetPosition(0.0f, 0.0f);
}

void GlowGo::Update(float dt)
{
	/*
	UINT8 alpha = Utils::Lerp(255, 0, (flowTime / duration));
	sprite.setColor({ 255, 255, 255, alpha });

	if (flowTime > duration)
	{
		if (pool != nullptr)
		{
			SCENE_MGR.GetCurrScene()->RemoveGo(this);
			pool->Return(this);
		}
		else
		{
			SetActive(false);
		}
	}
	*/
}

void GlowGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, sf::BlendAdd);
}

void GlowGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
}

void GlowGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
}

void GlowGo::SetOrigin(Origins origin)
{
	const sf::Texture* tex = sprite.getTexture();

	if (!tex)
	{
		std::cerr << "GlowGo SetOrigin Error" << std::endl;
		return;
	}
	sf::Vector2f originPos(sprite.getTexture()->getSize());
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	sprite.setOrigin(originPos);
}

void GlowGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void GlowGo::SetTexture(const std::string& id)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
	textureId = id;
}