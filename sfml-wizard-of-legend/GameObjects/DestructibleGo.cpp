#include "stdafx.h"
#include "DestructibleGo.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"
#include "SceneGame.h"
#include "Particle.h"

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
	sf::Vector2f originPos(sprite.getTexture()->getSize());
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	sprite.setOrigin(originPos);
}

void DestructibleGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
}

void DestructibleGo::SetScale(float x, float y)
{
	sprite.setScale(x, y);
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

void DestructibleGo::SetTexture(const std::string& id)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
}


void DestructibleGo::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	sprite.setTextureRect(rect);
}

void DestructibleGo::OnHitDestructibleGo(int hit)
{
	durability -= hit;
	if (durability <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);

		//SetParticle(position, scale);
	}
}

void DestructibleGo::SetParticle(sf::Vector2f position)
{
	Particle* particle = particlePool->Get();
	particle->SetPosition(position.x, position.y);
	//particle->sprite.setRotation(randomValue * 10);

	SCENE_MGR.GetCurrScene()->AddGo(particle);
}

void DestructibleGo::SetParticlePool(ObjectPool<Particle>* pool)
{ 
	particlePool = pool; 
};