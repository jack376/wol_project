#include "stdafx.h"
#include "BreakableObj.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"
#include "SceneGame.h"
#include "Particle.h"
#include "Player.h"

void BreakableObj::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr) { sprite.setTexture(*tex); }

	float scaleFactor = sprite.getScale().x;

	float colliderW = sprite.getTextureRect().width * scaleFactor;
	float colliderH = sprite.getTextureRect().height * scaleFactor;

	collider = sf::FloatRect(position.x, position.y, colliderW, colliderH);

	SetOrigin(origin);
}

void BreakableObj::Update(float dt)
{
	if (player == nullptr)
	{
		return;
	}

	hitTimer += dt;
	if (this->name == name && hitTimer > hitCooldown)
	{
		if (collider.intersects(player->rect.getGlobalBounds()))
		{
			hitTimer = 0.0f;
			OnHitBreakableObj(name, 2);
		}
	}

	shape.setPosition(collider.left, collider.top);
	shape.setSize(sf::Vector2f(collider.width, collider.height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(0.25f);
}

void BreakableObj::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	//window.draw(shape);
}

void BreakableObj::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
	shape.setPosition(p);
}

void BreakableObj::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
	shape.setPosition(position);
}

void BreakableObj::SetOrigin(Origins origin)
{
	sf::Vector2f originPos(sprite.getTexture()->getSize());
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	sprite.setOrigin(originPos);
	shape.setOrigin(originPos);
}

void BreakableObj::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
	shape.setScale(x, y);
}

void BreakableObj::SetScale(float x, float y)
{
	sprite.setScale(x, y);
	shape.setScale(x, y);
}

void BreakableObj::SetFlipX(bool flip)
{
	sprite.setScale((flip) ? -abs(sprite.getScale().x) : abs(sprite.getScale().x), sprite.getScale().y);
}

void BreakableObj::SetFlipY(bool flip)
{
	sf::Vector2f scale = sprite.getScale();
	scale.y = abs(scale.y);
	scale.y = (flip) ? scale.y : -scale.y;
	sprite.setScale(scale);
}

void BreakableObj::SetTexture(const std::string& id)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
}


void BreakableObj::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	sprite.setTextureRect(rect);
}

void BreakableObj::SetParticle(sf::Vector2f position, int count)
{
	for (int i = 0; i < count; i++)
	{
		Particle* particle = particlePool->Get();
		particle->SetPosition(collider.width / 2 + position.x, collider.height / 2 + position.y);
		SCENE_MGR.GetCurrScene()->AddGo(particle);
	}
}

void BreakableObj::SetParticlePool(ObjectPool<Particle>* pool)
{ 
	particlePool = pool; 
};

void BreakableObj::OnHitBreakableObj(const std::string& name, int damage)
{
	durability -= damage;
	if (durability <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
		SetParticle(position, 50);
		if (sceneGame != nullptr)
		{
			sceneGame->DestroyBreakableObj(this);
		}
	}
}