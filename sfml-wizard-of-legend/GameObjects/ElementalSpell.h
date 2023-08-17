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
	// ����� �ϵ��ڵ������� �����͸� �Ѱ� �޴� ������ ����
	SceneGame* scene;
	Player* player;
	Monster* monster;
	Beam raycaster;


	AnimationController anim;
	
	BoxCollider2D* Collider;
	SpriteGo* collider;

	std::queue <float> comboQueue;


	// ���� ���� sf::Vector2f
	sf::Vector2f dir;



	// ��ġ ���� sf::Vector2f
	sf::Vector2f spawnPos;



	float speed;


	// ���� ����

	bool isCombo = false;

	// ��ų�� �����Ǿ����� ����
	bool isSpawn = false;

	// ���� �浹
	bool isCol = false;

	// ��ų�� �����ߴ��� ���� / �ߺ�����
	bool isAttack = false;
	
	// �ߺ� ���� ����
	float attackTimer = 0.f;
	float attackDuration = 0.5f;

	// �޺� Ÿ�̸� ����
	float prevComboTime = -100.f;

	float comboTimer = 0.f;
	float comboDuration = 0.6f;

	// �޺� Ƚ��
	int attackCount = 0;

	// �ִϸ��̼� ����
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

