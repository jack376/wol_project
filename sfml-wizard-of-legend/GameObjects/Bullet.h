#pragma once
#include "SpriteGo.h"
#include "BoxCollider2D.h"

class Player;

class Bullet :
    public SpriteGo
{
protected:
	sf::Vector2f direction;
	float speed = 0.f;
	int damage = 0;
	bool isAttacked = false;

	Player* player = nullptr;
	BoxCollider2D collider;

public:
	Bullet(const std::string& textureId = "", const std::string& n = "");
	virtual ~Bullet() override;

	virtual void Init() override; 
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetPlayer(Player* player) { this->player = player; }
	void SetDamage(int num) { damage = num; }
	void SetSpeed(float num) { speed = num; }

	int GetDamage() { return damage; }
	float GetSpeed() { return speed; }

	void Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed, int damage);
};

