#pragma once
#include "Monster.h"
#include "ObjectPool.h"
#include "AnimationProjectile.h"


enum class FireBossAttackPattern
{
	Jump,
	Fire,
	Meteor,
	Dash,
	Kick,
};
class FireBoss :
    public Monster
{
protected:
	std::vector<int> randomNums;
	std::vector<AnimationProjectile*> fireballs;
	std::vector<sf::Vector2f> fireballPoss;

	sf::Vector2f jumpDownPos;
	sf::Vector2f jumpUpPos;

	int patternCount = 0;
	int fireCount = 0;
	int shotCount = 0;
	float fireballRate = 0.5f;
	float fireballTimer = 0.5f;
	float meteoJumpDuration = 1.f;
	float jumpDuration = 0.5f;
	float jumpTimer = 0.f;
	float patternDelayRate = 2.f;
	float patternDelayTimer = 0.f;
	bool stateStart = false;
	bool patternStart = false;
	bool isDelay = false;

	FireBossAttackPattern currentAttackPattern;
	ObjectPool<AnimationProjectile> projectilePool;
public:

	FireBoss(MonsterId id, const std::string& textureId = "", const std::string& n = "FireBoss");
	virtual ~FireBoss() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;
	virtual void SetActive(bool active) override;

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
	void Meteor(float dt);
	void SetAttackPattern(FireBossAttackPattern pattern);

	virtual void OnAttacked(float damage) override;
	std::vector<sf::Vector2f> CalculateProjectilePositions(const sf::Vector2f& playerPosition, const sf::Vector2f& monsterPosition, float radius, int count, float angleRange);


};

