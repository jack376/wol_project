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

enum class AttactType
{
	Melee,
	Ranged,
};

struct MonsterStat
{
	std::string name;
	AttactType type;
	int maxHp;
	float speed;
	int damage;
	float attackRate;
	float attackRange;	//공격 범위
	float searchRange;	//탐색 범위
};

enum class MonsterId
{
	Ghoul,
	GhoulLarge,
	Lancer,
};


class Monster :
    public SpriteGo
{
protected:
	MonsterId monsterId;
	MonsterState currentState = MonsterState::Idle;
	AnimationController animation;

	MonsterStat stat;
	int hp = 0;
	float attackTimer = 0.f;
	float knockBackTime = 0.15f;
	float knockBackTimer = 0.f;
	bool isAttacked = false;

	Player* player = nullptr;

	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction; //이동하는 방향

	sf::CircleShape searchRange;
	sf::CircleShape attackRange;

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

	void Idle();
	void Attack(float dt);
	void Move(float dt);
	void Die();
	void KnockBack();
	
	void SetLook(sf::Vector2f playerPos);
	void SetPlayer(Player* player) { this->player = player; }
	void OnAttacked(float damage);
	void HandleBehavior(float dt);
};


//생성
//배치(스폰)
//탐색(플레이어)
//이동 및 공격
