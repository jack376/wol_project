#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "BoxCollider2D.h"
#include "SpriteEffect.h";

class Player;
class Tile;

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
	float attackRange;	//���� ����
	float searchRange;	//Ž�� ����
};

enum class MonsterId
{
	Ghoul,
	GhoulLarge,
	Lancer,
	Archer,
};

class SceneGame;

class Monster :
    public SpriteGo
{
protected:
	MonsterId monsterId;
	MonsterState currentState = MonsterState::Idle;
	MonsterStat stat;
	AnimationController animation;
	SpriteEffect attackEffect;

	int hp = 0;
	float attackTimer = 0.f;
	float knockBackTime = 0.15f;
	float knockBackTimer = 0.f;
	bool isAttacked = false;
	bool isAttacking = false;
	bool isAwake = false;

	Player* player = nullptr;
	Tile* currentTile = nullptr;

	sf::Vector2f look; //�ٶ󺸴� ����
	sf::Vector2f direction; //�̵��ϴ� ����

	std::vector<std::vector<Tile*>>* wouldTiles = nullptr;

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
	void HandleState(float dt);

	void Idle();
	virtual void Attack(float dt);
	void Move(float dt);
	void Die();
	void KnockBack();
	
	void SetLook(sf::Vector2f playerPos);
	void SetPlayer(Player* player) { this->player = player; }
	void SetRectBox();
	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->wouldTiles = tiles; }

	void OnAttacked(float damage);
	void HandleBehavior(float dt);

	void CalculatorCurrentTile();
	std::vector<Tile*> CalculatorRangeTiles(int row, int col);
};
