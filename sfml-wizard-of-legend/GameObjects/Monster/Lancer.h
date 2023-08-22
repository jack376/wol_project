#pragma once
#include "Monster.h"
#include "CustomEffect.h"
#include "AnimationController.h"
#include "Beam.h"
#include "BoxCollider2D.h"

class Lancer :
    public Monster
{
protected:
	sf::Sprite spear;
	
	AnimationController spearAni;
	AttackState currentAttackState;
	BoxCollider2D collider;

	float aimRate = 1.f;
	float aimTimer = 0.f;
	float shootRate = 1.f;
	float shootTimer = 0.f;
	bool isEffect = false;

public:
	Lancer(MonsterId id, const std::string& textureId = "", const std::string& n = "Lancer");
	virtual ~Lancer() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void HandleAttackState(float dt);

	virtual void Attack(float dt);
	void Aim(float dt);
	void Shoot(float dt);
};

