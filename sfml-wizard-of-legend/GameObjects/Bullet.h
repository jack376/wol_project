#pragma once
#include "SpriteGo.h"

class Player;

class Bullet :
    public SpriteGo
{
protected:
	sf::Vector2f direction;
	float speed = 0.f;
	float range = 2000.f;
	int damage = 0;

	Player* player;

public:
	Bullet(const std::string& textureId = "", const std::string& n = "");
	virtual ~Bullet() override;

	virtual void Init() override; 
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetDamage(int num) { damage = num; }
	int GetDamage() { return damage; }

	void Fire(const sf::Vector2f pos, const sf::Vector2f direction, float speed);
};

