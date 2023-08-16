#include "stdafx.h"
#include "Particle.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"
#include "SceneGame.h"

void Particle::Reset()
{
	// Texture Load
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr) { sprite.setTexture(*tex); }

	// Reset Member Variable
	flowTime = 0.0f;
	velocity = 0.0f;
	duration = 0.75f;
	speed    = 750.0f;

	float randomVal = Utils::RandomRange(0.2f, 1.0f);
	sf::Vector2f randomDir = Utils::RandomOnCircle(1.0f);

	direction  = randomDir;
	speed     *= randomVal;
	rotation   = randomVal;
	duration  *= randomVal;

	SetOrigin(origin);
	SetRotation(rotation * 360.0f);
	SetPosition(0.0f, 0.0f);
}

void Particle::Update(float dt)
{		
	flowTime += dt;
	speed -= slowdown * dt;
	velocity += gravity * flowTime;

	position += direction * speed * dt;
	sprite.setPosition(position.x, position.y + velocity);

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
}

void Particle::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, sf::BlendAdd);
}

void Particle::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
}

void Particle::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
}

void Particle::SetOrigin(Origins origin)
{
	const sf::Texture* tex = sprite.getTexture();

	if (!tex)
	{
		std::cerr << "Particle SetOrigin Error" << std::endl;
		return;
	}
	sf::Vector2f originPos(sprite.getTexture()->getSize());
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	sprite.setOrigin(originPos);
}

void Particle::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void Particle::SetRotation(float angle)
{
	sprite.setRotation(angle);
}

void Particle::SetTexture(const std::string& id)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
	textureId = id;
}

void Particle::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	sprite.setTextureRect(rect);
}