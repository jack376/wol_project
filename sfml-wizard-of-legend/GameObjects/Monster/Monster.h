#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "BoxCollider2D.h"
#include "CustomEffect.h";
#include "Beam.h"

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

enum class AttackState
{
	Aim,
	Shoot,
	Cool,
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
	Archer,
	Mage,
};

class SceneGame;
using Pair = std::pair<int, int>;

class Monster :
    public SpriteGo
{
protected:
	MonsterId monsterId;
	MonsterState currentState = MonsterState::Idle;
	MonsterStat stat;
	AnimationController animation;

	CustomEffect attackEffect;
	//ObjectPool<SpriteEffect> hitEffects;

	int hp = 0;
	float attackTimer = 0.f;
	float knockBackTime = 0.15f;
	float knockBackTimer = 0.f;
	float pathUpdateRate = 1.f;
	float pathUpdateTimer = 0.f;
	bool isAttacked = false;
	bool isAwake = false;

	Player* player = nullptr;
	Tile* currentTile = nullptr;
	Tile* nextTile = nullptr;

	sf::Vector2f look; //바라보는 방향
	sf::Vector2f direction; //이동하는 방향
	sf::Vector2f attackDir;
	sf::Vector2f prevPos;
	

	std::vector<std::vector<Tile*>>* tilesWorld = nullptr;
	std::vector<Tile*>* nongroundTiles = nullptr;
	std::vector<std::vector<int>>* intMap = nullptr;

	sf::CircleShape searchRange;
	sf::CircleShape attackRange;

	Beam raycaster;
	std::pair<bool, std::stack<Pair>> path;

	using Pair = std::pair<int, int>;
	using pPair = std::pair<double, Pair>;
public:
	Monster(MonsterId id, const std::string& textureId = "", const std::string& n = "");
	virtual ~Monster() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

    void SetState(MonsterState newState);
	virtual void HandleState(float dt);

	virtual void Idle();
	virtual void Attack(float dt);
	virtual void Move(float dt);
	void Die();
	virtual void KnockBack(float dt);

	
	
	const sf::Vector2f SetLook(sf::Vector2f playerPos);
	void SetPlayer(Player* player) { this->player = player; }
	void SetRectBox();
	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->tilesWorld = tiles; }
	void SetIntMap(std::vector<std::vector<int>>* intMap) { this->intMap = intMap; }
	void SetNonGroundTiles(std::vector<Tile*>* tiles) { nongroundTiles = tiles; }

	void OnAttacked(float damage);

	void CalculatorCurrentTile();
	std::vector<Tile*> CalculatorRangeTiles(int row, int col);
};
