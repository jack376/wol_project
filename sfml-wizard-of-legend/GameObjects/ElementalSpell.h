#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "BoxCollider2D.h"
#include "Beam.h"
#include "ObjectPool.h"
#include "Skill.h"
#include "SceneMgr.h"

class SpriteGo;
class SceneGame;
class Player;
class Monster;
class Tile;
class SpriteEffect;

class ElementalSpell : public SpriteGo
{
protected:
	// ����� �ϵ��ڵ������� �����͸� �Ѱ� �޴� ������ ����
	SceneGame* scene;
	Player* player;
	Monster* monster;
	std::list<Monster*> monsters;
	std::list<Monster*> colMonsters;
	Beam raycaster;

	ObjectPool<ElementalSpell> pool;
	ObjectPool<SpriteEffect> monsterHitEffectPool;

	AnimationController anim;
	
	BoxCollider2D Collider;


	ElementTypes currentElementType = ElementTypes::None;
	SkillTypes currentSkillType = SkillTypes::None;
	RangeTypes currentRangeType = RangeTypes::None;
	SkillEvents currentEvent = SkillEvents::None;

	SpellInfo currentInfo;


	std::queue <float> comboQueue;

	std::string currentAnimId;


	// ���� ���� sf::Vector2f
	sf::Vector2f dir;

	// ��ġ ���� sf::Vector2f
	sf::Vector2f spawnPos;



	// ���� ����

	bool isCombo = false;

	// ��ų�� �����Ǿ����� ����
	bool isSpawn = false;

	// ���� �浹
	bool isCol = false;

	// ��ų�� �����ߴ��� ���� / �ߺ�����
	bool isAttack = false;
	
	// �޺� Ƚ��
	int attackCount = 0;

	int damage;

	// �ߺ� ���� ����
	float attackTimer = 0.f;
	float attackDuration = 2.0f;

	// �޺� Ÿ�̸� ����
	float prevComboTime = -100.f;

	float comboTimer = 0.f;
	float comboDuration = 0.6f;

	float speed = 500.f;
	float curveSpeed = 12.f;
	float range;


	// �ִϸ��̼� ����
	float angle = 0.f;
	float addAngle;

	// �밢�� sin�
	float amplitude = 8.f;
	float frequency = 1.2f;


	// �ӽ�
	//sf::Vector2f axis(400.0f, 300.0f); // �� ��ġ
	float curveAngle = 45.0f; // �� ���� (��)
	
	float time = 0.0f; // dt�� �����Ǵ� �ð�



	Tile* currentTile = nullptr;
	std::vector<std::vector<Tile*>>* worldTiles = nullptr;

public:
	ElementalSpell(const std::string& textureId = "", const std::string& n = "");
	virtual ~ElementalSpell() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void MeleeUpdate(float dt);
	void RangeUpdate(float dt);
	void CurveUpdate(float dt);
	void StraightUpdate(float dt);

	//void SetSpell(SpellInfo skillInfo);

	void SetScene(SceneGame* scene) { this->scene = scene; }
	void SetPlayer(Player* player) { this->player = player; }
	void SetMonster(Monster* monster) { this->monster = monster; }
	void SetMonsterList(std::list<Monster*>& monsters) { this->monsters = monsters; }
	void SetPool(ObjectPool<ElementalSpell>& pool) { this->pool = pool; }
	
	void SetElementType(ElementTypes type) { currentElementType = type; }
	void SetSkillType(SkillTypes type) { currentSkillType = type; }
	void SetRangeType(RangeTypes type) { currentRangeType = type; }
	void SetSpellInfo(SpellInfo info) { currentInfo = info; }
	void SetAngle(float angle) { this->angle = angle; }
	void SetDir(sf::Vector2f dir) { this->dir = dir; }

	sf::RectangleShape& GetCollider();

	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->worldTiles = tiles; }

	sf::Vector2f  CalAxisSin(float time, float speed, float frequency, float amplitude, const sf::Vector2f& axis, float angleInDegrees);
	void CalculatorCurrentTile();
	std::vector<Tile*> CalculatorRangeTiles(int row, int col);
};
