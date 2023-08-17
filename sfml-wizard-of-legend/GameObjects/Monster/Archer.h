#pragma once
#include "Monster.h"
#include "Bullet.h"
#include "Beam.h"

enum class ArcherAttackState
{
	Aim,
	Shoot,
	Cool,
};

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
	Beam bulletLine;
	ArcherAttackState currentAttackState;

	float aimRate = 1.f;
	float aimTimer = 0.f;
	float arrowSpeed = 1000.f;
public:
	Archer(MonsterId id, const std::string& textureId = "", const std::string& n = "Archer");
	virtual ~Archer() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void HandleAttackState(float dt);

	virtual void Attack(float dt) override;
	void Aim(float dt);
	void Shoot(float dt);
};

