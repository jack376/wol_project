#pragma once
#include "Monster.h"
#include "ObjectPool.h"
#include "AnimationProjectile.h"

enum class FireBossAttackPattern
{
	Jump,
	Dash,
	Kick,
	Fire,
};
class FireBoss :
    public Monster
{
protected:
	std::vector<int> randomNums;

	int patternCount = 0;
	int fireCount = 0;

	FireBossAttackPattern currentAttackPattern;
	ObjectPool<AnimationProjectile> projectilePool;
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
	void SetAttackPattern(FireBossAttackPattern pattern);

	virtual void OnAttacked(float damage);
};

