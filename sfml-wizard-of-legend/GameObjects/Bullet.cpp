#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"
#include "SceneMgr.h"
#include "BoxCollider2D.h"

Bullet::Bullet(const std::string& textureId, const std::string& n)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	SpriteGo::Init();
	collider.Init();
	SetOrigin(Origins::MC);
}

void Bullet::Release()
{
	SpriteGo::Release();
	collider.Release();
}

void Bullet::Reset()
{
	SpriteGo::Reset();
	collider.Reset();

	sprite.setScale({ 4, 4 });
	sprite.setRotation(0.f);

	collider.SetSprite(sprite);

	SetPosition(0.f, 0.f);
	speed = 0.f;
	direction = { 0.f, 0.f };
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);
	if (collider.GetActive())
		collider.Update(dt);

	position += direction * speed * dt;
	SetPosition(position);
	collider.SetPosition(position);

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

void Bullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if (collider.GetActive())
		collider.Draw(window);
}

void Bullet::Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed, int damage)
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