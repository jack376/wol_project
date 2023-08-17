#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "BoxCollider2D.h"
#include "Beam.h"

class SpriteGo;
class SceneGame;
class Player;
class Monster;
class BoxCollider2D;
class Tile;

class ElementalSpell : public SpriteGo
{
protected:
	// 현재는 하드코딩이지만 데이터를 넘겨 받는 식으로 수정
	SceneGame* scene;
	Player* player;
	Monster* monster;
	Beam raycaster;


	AnimationController anim;
	
	BoxCollider2D* Collider;
	SpriteGo* collider;

	std::queue <float> comboQueue;


	// 방향 벡터 sf::Vector2f
	sf::Vector2f dir;



	// 위치 벡터 sf::Vector2f
	sf::Vector2f spawnPos;



	float speed;


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
	float attackDuration = 0.5f;

	// 콤보 타이머 설정
	float prevComboTime = -100.f;

	float comboTimer = 0.f;
	float comboDuration = 0.6f;

	// 콤보 횟수
	int attackCount = 0;

	// 애니메이션 각도
	float angle = 0.f;

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


	//void SetSpell(SpellInfo skillInfo);

	void SetScene(SceneGame* scene) { this->scene = scene; }
	void SetPlayer(Player* player) { this->player = player; }
	void SetMonster(Monster* monster) { this->monster = monster; }

	sf::RectangleShape& GetCollider() const { return collider->rect; }

	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->wouldTiles = tiles; }
	void CalculatorCurrentTile();
	std::vector<Tile*> CalculatorRangeTiles(int row, int col);

};

