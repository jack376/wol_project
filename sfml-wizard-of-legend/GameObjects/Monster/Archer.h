#pragma once
#include "Monster.h"
class Archer :
    public Monster
{
protected:
	sf::Sprite bow;
	AnimationController bowAni;
	float chargeRate = 1.f;
	float chargeTimer = 0.f;
public:
	Archer(MonsterId id, const std::string& textureId = "", const std::string& n = "Archer");
	virtual ~Archer() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void Attack();
};

