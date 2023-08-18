#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "BoxCollider2D.h"
#include "Beam.h"
#include "ObjectPool.h"

class SpriteGo;
class SceneGame;
class Player;
class Monster;
class Tile;

class ElementalSpell : public SpriteGo
{
protected:
	// 현재는 하드코딩이지만 데이터를 넘겨 받는 식으로 수정
	SceneGame* scene;
	Player* player;
	Monster* monster;
	std::list<Monster*> monsters;
	std::list<Monster*> colMonsters;
	Beam raycaster;

	ObjectPool<ElementalSpell> pool;

	AnimationController anim;
	
	BoxCollider2D Collider;


	ElementTypes currentElementType = ElementTypes::None;
	SkillTypes currentSkillType = SkillTypes::None;
	RangeTypes currentRangeType = RangeTypes::None;
	SkillEvents currentEvent = SkillEvents::None;

	sf::Keyboard::Key currentKey = sf::Keyboard::Unknown;
	sf::Mouse::Button currentButton = sf::Mouse::None;

	std::queue <float> comboQueue;

	std::vector<std::string> currentAnimId;


	// 방향 벡터 sf::Vector2f
	sf::Vector2f dir;

	// 위치 벡터 sf::Vector2f
	sf::Vector2f spawnPos;



	// 상태 변수

	bool isCombo = false;

	// 스킬이 생성되었는지 여부
	bool isSpawn = false;

	// 몬스터 충돌
	bool isCol = false;

	// 스킬이 공격했는지 여부 / 중복방지
	bool isAttack = false;
	
	// 중복 공격 방지
	float attackTimer = 0.f;
	float attackDuration = 2.0f;

	// 콤보 타이머 설정
	float prevComboTime = -100.f;

	float comboTimer = 0.f;
	float comboDuration = 0.6f;

	float speed = 500.f;

	float curveSpeed = 12.f;


	// 콤보 횟수
	int attackCount = 0;

	// 애니메이션 각도
	float angle = 0.f;

	// 대각선 sin곡선
	float amplitude = 8.f;
	float frequency = 1.2f;


	// 임시
	//sf::Vector2f axis(400.0f, 300.0f); // 축 위치
	float curveAngle = 45.0f; // 축 각도 (도)
	
	float time = 0.0f; // dt가 누적되는 시간

	Tile* currentTile = nullptr;
	std::vector<std::vector<Tile*>>* wouldTiles = nullptr;

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

	sf::RectangleShape& GetCollider();

	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->wouldTiles = tiles; }

	sf::Vector2f  CalAxisSin(float time, float speed, float frequency, float amplitude, const sf::Vector2f& axis, float angleInDegrees);
	void CalculatorCurrentTile();
	std::vector<Tile*> CalculatorRangeTiles(int row, int col);

};

