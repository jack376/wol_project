#include "stdafx.h"
#include "Projectile.h"
#include "Player.h"
#include "SceneMgr.h"
#include "BoxCollider2D.h"

Projectile::Projectile(const std::string& textureId, const std::string& n)
{
}

Projectile::~Projectile()
{
}

void Projectile::Init()
{
	SpriteGo::Init();
	collider.Init();
	SetOrigin(Origins::MC);
}

void Projectile::Release()
{
	SpriteGo::Release();
	collider.Release();
}

void Projectile::Reset()
{
	SpriteGo::Reset();
	collider.Reset();

	SetPosition(0.f, 0.f);
	sprite.setRotation(0.f);
	sprite.setScale({ 4, 4 });

	collider.SetSprite(sprite);

	speed = 0.f;
	direction = { 0.f, 0.f };
}

void Projectile::Update(float dt)
{
	SpriteGo::Update(dt);

	position += direction * speed * dt;
	SetPosition(position);
	collider.SetPosition(position);

	if (collider.GetActive())
		collider.Update(dt);

	if (isActive && player != nullptr && player->IsAlive())
	{
		if (collider.ObbCol(player->rect))
		{
			player->SetHitDir(direction);
			player->SetHp(-damage);

			SetActive(false);
			collider.SetActive(false);
		}
	}
}

void Projectile::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if (collider.GetActive())
		collider.Draw(window);
}

void Projectile::Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed, int damage)
{
	SetActive(true);
	sprite.setRotation(Utils::Angle(direction) + 90);
	SetPosition(pos);

	collider.SetActive(true);
	collider.GetObbCol().setRotation(Utils::Angle(direction) + 90);
	collider.SetOrigin(Origins::BC);

	this->direction = direction;
	this->speed = speed;
	this->damage = damage;
}