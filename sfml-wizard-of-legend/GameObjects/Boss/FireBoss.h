#pragma once
#include "Monster.h"

enum class FireBossAttackPattern
{
	Jump,
	Dash,
	Kick,
	Shoot,
};
class FireBoss :
    public Monster
{
protected:
	int patternCount = 0;
	FireBossAttackPattern currentAttackState;

public:
	FireBoss(MonsterId id, const std::string& textureId = "", const std::string& n = "");
	virtual ~FireBoss() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void HandleState(float dt);
	void HandleAttackPattern(float dt);


	virtual void Start();
	virtual void Idle();
	virtual void Attack(float dt);
	virtual void Die();
	virtual void KnockBack(float dt);

	void Dash(float dt);
	void Jump(float dt);
	void Kick(float dt);
	void Fire(float dt);

	virtual void OnAttacked(float damage);
};

