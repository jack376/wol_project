#include "stdafx.h"
#include "Bullet.h"
#include "Player.h"

Bullet::Bullet(const std::string& textureId, const std::string& n)
{
}

Bullet::~Bullet()
{
}

void Bullet::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Bullet::Release()
{
	SpriteGo::Release();

}

void Bullet::Reset()
{
	SpriteGo::Reset();

	sprite.setScale({ 4, 4 });
	sprite.setRotation(0.f);
	SetPosition(0.f, 0.f);
	speed = 0.f;
	direction = { 0.f, 0.f };
	range = 2000.f;
}

void Bullet::Update(float dt)
{
	SpriteGo::Update(dt);

	range -= speed * dt; // 이동량 빼기
	if (range <= 0.f) //사거리까지 이동하면 총알 삭제
	{
		SetActive(false);
		return;
	}

	position += direction * speed * dt;
	SetPosition(position);

	if (player != nullptr && player->IsAlive())
	{
		if (sprite.getGlobalBounds().intersects(player->sprite.getGlobalBounds()))
		{
			player->SetHp(damage);
			SetActive(false);
		}
	}
}

void Bullet::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void Bullet::Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed)
{
	SetActive(true);
	sprite.setRotation(Utils::Angle(direction) + 90);
	SetPosition(pos);

	this->direction = direction;
	this->speed = speed;
}