#pragma once
#include "Monster.h"
#include "AnimationProjectile.h"
#include "Beam.h"

class Mage :
    public Monster
{
protected:
    std::array<AnimationProjectile, 5> fireballs;
    AttackState currentAttackState;

    float aimRate = 1.f;
    float aimTimer = 0.f;
    float fireballSpeed = 1000.f;
	float fireballCreateRadius = 50.f;
public:
	Mage(MonsterId id, const std::string& textureId = "", const std::string& n = "Archer");
	virtual ~Mage() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void HandleAttackState(float dt);

	virtual void Attack(float dt) override;
	void Aim(float dt);
	void Shoot(float dt);

	std::vector<sf::Vector2f> CalculateProjectilePositions(const sf::Vector2f& playerPosition, const sf::Vector2f& monsterPosition, float radius, int count);
};

