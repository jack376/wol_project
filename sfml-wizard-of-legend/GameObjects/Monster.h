#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

class Player;

enum class MonsterState 
{
	Idle,
	Moving,
	Attacking,
	Dead,
	KnockBack,
};

enum class MonsterId
{
	Ghoul,
	Knight,
};


class Monster :
    public SpriteGo
{
protected:
	MonsterId monsterId;
	MonsterState currentState = MonsterState::Idle;
	AnimationController animation;

	MonsterInfo stat;
	int hp;
	float attackTimer = 0.f; //timer가 rate보다 커지면 공격 타이밍

	Player* player = nullptr;

	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction; //이동하는 방향

public:
	Monster(MonsterId id, const std::string& textureId = "", const std::string& n = "");
	virtual ~Monster() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

    void SetState(MonsterState newState);
	void HandleState();

	virtual void Attack();
	void Move(float dt);
	void Die();
	
	void SetLook(sf::Vector2f playerPos);
	void SetPlayer(Player* player) { this->player = player; }
	void HandleBehavior(float dt);
};


//생성
//배치(스폰)
//탐색(플레이어)
//이동 및 공격
