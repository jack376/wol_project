#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "BoxCollider2D.h"
#include "ObjectPool.h"
#include "ElementalSpell.h"

enum class Dir
{
	UpRight,
	UpLeft,
	DownRight,
	DownLeft,
	Up,
	Right,
	Down,
	Left
};

enum class AttackDir
{
	None = -1,
	Up,
	Right,
	Down,
	Left,
};

enum class HitDir
{
	None = -1,
	Up,
	Right,
	Down,
	Left
};

enum class States
{
	Idle,
	Run,
	Dash,
	Slide,
	Attack,
	Hit,
	Fall,
	Die,
};



class SceneGame;
class Monster;
class Tile;

class Player : public SpriteGo
{
private:
	AnimationController anim;

	States currentState = States::Idle;
	Dir currentDir = Dir::Down;
	Dir slideDir = Dir::Down;
	AttackDir attackDir = AttackDir::None;
	HitDir hitDir = HitDir::None;
	SkillEvents sEvent = SkillEvents::None;

	ObjectPool<ElementalSpell> spellPool;

	SceneGame* scene;
	Monster* monster;
	std::list<Monster*> monsters;
	SpriteGo* dirIcon;
	SpriteGo* portal;

	sf::CircleShape attackPosCol;	// ��ų ���� ���� ����ȭ

	// Player Shader
	sf::Shader shader;
	sf::Texture paletteTexture;

	//BoxCollider2D obb;

	// ��ų Ŭ����
	//4���� �����̶� ��
	// ��ų �Ŵ��� ���� swap�Լ� ��������
	//Skill fireball;
	//Skill wdash;
	//Skill wslash;

	int maxHp = 100;
	int hp = 100;

	// ���� ����
	int attackCount = 0;
	int attackComboCount = 0;	// ���� ���� Ƚ��

	// ���ڿ� �߰� ����
	int attackNameInsertPos = 6;

	// ���� sf::Vector2f
	sf::Vector2f dir;
	sf::Vector2f dashDir;
	sf::Vector2f look;
	sf::Vector2f iconLook;

	sf::Vector2f hitLook;

	// ��ǥ sf::Vector2f
	std::vector<sf::Vector2f> destPos;
	sf::Vector2f dashStart;
	sf::Vector2f dashDest;

	sf::Vector2f fallStart;
	sf::Vector2f fallDest;

	sf::Vector2f attackPos;

	sf::Vector2f prevPos;

	// �÷��̾� ��
	sf::Color playerColor;

	float originAngle = 0.f;

	float playerLookAngle = 0.f;
	float hitLookAngle = 0.f;

	float debugTimer = 0.f;
	float debugDuration = 1.f;

	float attackDistance = 100.f;

	float speed = 600.f;
	float dashDistance = 600.f;

	float dashTimer = 0.f;
	float dashDuration = 0.3f;

	float fallTimer = 0.f;
	float fallDuration = 0.4f;

	float fallHitTimer = 0.f;
	float fallHitDuration = 1.0f;



	// �����̵� ���� ������ȯ ������
	float slideDuration = 0.3f;
	float slideTimer = 0.f;

	float dashCoolTimer = 0.f;
	float dashCoolDuration = 0.3f;

	float hitTimer = 0.f;
	float hitDuration = 0.3;

	float portalTimer = 0.f;
	float portalDuration = 0.5;
	float portalTermTimer = 0.f;



	// ���º���
	bool isAlive = true;
	bool isRun = false;
	bool isDash = false;
	bool isSlide = false;
	bool isDashCool = false;
	bool isAttack = false;
	bool isHit = false;
	bool isHitAnim = false;
	bool isInvincible = false;
	bool isDieAnim = false;
	bool isFalling = false;
	bool isFallHit = false;
	bool isPortalBigAnim = false;
	bool isPortalSmallAnim = false;
	bool isPortalAnimTerm = false;

	bool isMove = false;
	bool isDashing = false;


	std::vector<std::string> idleId;
	std::vector<std::string> runId;
	std::vector<std::string> dashId;
	std::vector<std::string> slideId;
	std::vector<std::string> attackId;
	std::vector<std::string> hitId;
	std::vector<std::string> fallId;
	//std::vector<std::string> daedId;

	std::string attackName;

	Tile* currentTile = nullptr;
	std::vector<std::vector<Tile*>>* wouldTiles = nullptr;

public:
	Player(const std::string& textureId = "", const std::string& n = "");
	virtual ~Player() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void IdleUpdate(float dt);
	void RunUpdate(float dt);
	void DashUpdate(float dt);
	void SlideUpdate(float dt);
	void AttackUpdate(float dt);
	void HitUpdate(float dt);
	void FallUpdate(float dt);
	void DieUpdate(float dt);

	void CalDir();
	void CalLookAngle();
	void CalHitLookAngle();



	void SetScene(SceneGame* scene) { this->scene = scene; }
	void SetMonster(Monster* monster) { this->monster = monster; }	// �ϵ� �ڵ���
	void SetMonsterList(std::list<Monster*>& monsters) { this->monsters = monsters; }
	void SetAttackPos();
	void SetDirIconPos();
	void SetDirIconDir();


	Monster* GetMonster() { return monster; }

	// ������ : -, ȸ�� : +
	void SetHp(int value);
	void SetHitDir(sf::Vector2f dir) { this->hitLook = dir; };

	// ��ų ���� ����
	sf::Vector2f GetAttackPos() { return attackPos; }
	float GetPlayerLookAngle() { return playerLookAngle; }
	// ��ų ���Ÿ� ����
	sf::Vector2f& GetLook() { return look; }
	// ��ų �̺�Ʈ
	SkillEvents GetSkillEvent() { return sEvent; }

	bool IsAlive() { return isAlive; }
	bool IsAttack() { return isAttack; }
	
	void ChangeState(States state);

	// �ִϸ��̼� �̸� ���
	void InsertAnimId();

	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->wouldTiles = tiles; }
	void CalculatorCurrentTile();
	std::vector<Tile*> CalculatorRangeTiles(int row, int col);

	void PortalAnimations(float dt);

	Tile* GetCurrentTile() { return currentTile; }
	const sf::FloatRect GetGlobalBounds() { return rect.getGlobalBounds(); }
	int* GetMaxHP() { return &maxHp; }
	int* GetHP() { return &hp; }
};