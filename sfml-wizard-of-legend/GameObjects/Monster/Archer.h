#pragma once
#include "Monster.h"
#include "Bullet.h"

class Archer :
    public Monster
{
protected:
	sf::Sprite bow;
	sf::Sprite attackArm;
	sf::Sprite pullArm;
	AnimationController bowAni;
	AnimationController attackArmAni;
	AnimationController pullArmAni;
	Bullet arrow;
	float ameRate = 1.f;
	float ameTimer = 0.f;
	float arrowSpeed = 1000.f;
	bool isAme = false;
	bool isFire = false;
public:
	Archer(MonsterId id, const std::string& textureId = "", const std::string& n = "Archer");
	virtual ~Archer() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void Attack(float dt);
};

