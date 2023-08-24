#include "stdafx.h"
#include "DecoGo.h"
#include "Utils.h"
#include "ResourceMgr.h"
#include "SceneMgr.h"
#include "SceneEditor.h"
#include "SceneGame.h"
#include "Particle.h"
#include "Player.h"

void DecoGo::Reset()
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	if (tex != nullptr) 
	{ 
		sprite.setTexture(*tex);
	}

	float scaleFactor = sprite.getScale().x;
	float colliderW = sprite.getTextureRect().width * scaleFactor;
	float colliderH = sprite.getTextureRect().height * scaleFactor;

	collider = sf::FloatRect(position.x, position.y, colliderW, colliderH);

	SetOrigin(origin);
}

void DecoGo::Update(float dt)
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
			OnHitDecoGo(name, 2);
		}
	}

	// Collider Test Code
	/*
	shape.setPosition(collider.left, collider.top);
	shape.setSize(sf::Vector2f(spriteRear.getTextureRect().width, spriteRear.getTextureRect().height));
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Red);
	shape.setOutlineThickness(0.25f);
	*/
}

void DecoGo::Draw(sf::RenderWindow& window)
{
	window.draw(sprite);
	window.draw(glow, sf::BlendAdd);

	//window.draw(shape);
}

void DecoGo::SetPosition(const sf::Vector2f& p)
{
	position = p;
	sprite.setPosition(p);
	shape.setPosition(p);
	glow.setPosition(p);
}

void DecoGo::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	sprite.setPosition(position);
	shape.setPosition(position);
	glow.setPosition(position);
}

void DecoGo::SetOrigin(Origins origin)
{
	sf::Vector2f originPos(collider.width, collider.height);
	originPos.x *= ((int)origin % 3) * 0.5f;
	originPos.y *= ((int)origin / 3) * 0.5f;

	sprite.setOrigin(originPos);
	shape.setOrigin(originPos);
	glow.setOrigin(originPos);
}

void DecoGo::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	sprite.setOrigin(x, y);
	shape.setOrigin(x, y);
	glow.setOrigin(x, y);
}

void DecoGo::SetScale(float x, float y)
{
	sprite.setScale(x, y);
	shape.setScale(x, y);
	glow.setScale(x, y);
}

void DecoGo::SetFlipX(bool flip)
{
	sprite.setScale((flip) ? -abs(sprite.getScale().x) : abs(sprite.getScale().x), sprite.getScale().y);
}

void DecoGo::SetFlipY(bool flip)
{
	sf::Vector2f scale = sprite.getScale();
	scale.y = abs(scale.y);
	scale.y = (flip) ? scale.y : -scale.y;
	sprite.setScale(scale);
}

void DecoGo::SetTexture(const std::string& id)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
}

void DecoGo::SetTextureRect(const sf::IntRect& rect)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	sprite.setTextureRect(rect);
}

void DecoGo::SetParticle(sf::Vector2f position, int count)
{
	for (int i = 0; i < count; i++)
	{
		Particle* particle = particlePool->Get();
		particle->SetPosition(collider.width / 2 + position.x, collider.height / 2 + position.y);
		SCENE_MGR.GetCurrScene()->AddGo(particle);
	}
}

void DecoGo::SetParticlePool(ObjectPool<Particle>* pool)
{ 
	particlePool = pool; 
};

void DecoGo::OnHitDecoGo(const std::string& name, int damage)
{
	durability -= damage;
	if (durability <= 0)
	{
		Scene* scene = SCENE_MGR.GetCurrScene();
		SceneGame* sceneGame = dynamic_cast<SceneGame*>(scene);
		SetParticle(position, 50);
		if (sceneGame != nullptr)
		{
			sceneGame->DestroyDecoGo(this);
		}
	}
}