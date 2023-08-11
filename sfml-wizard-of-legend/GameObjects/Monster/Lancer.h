#pragma once
#include "Monster.h"
class Lancer :
    public Monster
{
protected:
	SpriteEffect spear;
	float chargeRate = 1.f; 

public:
	Lancer(MonsterId id, const std::string& textureId = "", const std::string& n = "");
	virtual ~Lancer() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void Attack(float dt);
};

