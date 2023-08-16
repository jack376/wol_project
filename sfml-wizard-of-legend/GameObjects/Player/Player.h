#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"
#include "BoxCollider2D.h"
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
	Die,
};



class SceneGame;
class Monster;

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

	SceneGame* scene;
	Monster* monster;
	SpriteGo* dirIcon;

	sf::CircleShape attackPosCol;	// 스킬 생성 지점 가시화

	//BoxCollider2D obb;

	sf::Shader currentShader;
	sf::Sprite palette;
	sf::Texture paletteTexture;

	// 스킬 클래스
	//4개는 고정이란 뜻
	// 스킬 매니저 생성 swap함수 느낌으로
	//Skill fireball;
	//Skill wdash;
	//Skill wslash;

	int maxHp = 100;
	int hp = 100;

	// 공격 포즈
	int attackCount = 0;
	int attackComboCount = 0;	// 연속 공격 횟수

	// 문자열 중간 삽입
	int attackNameInsertPos = 6;

	// 방향 sf::Vector2f
	sf::Vector2f dir;
	sf::Vector2f dashDir;
	sf::Vector2f look;
	sf::Vector2f iconLook;

	sf::Vector2f hitLook;

	// 좌표 sf::Vector2f
	std::vector<sf::Vector2f> destPos;
	sf::Vector2f dashStart;
	sf::Vector2f dashDest;
	sf::Vector2f attackPos;

	// 플레이어 색
	sf::Color playerColor;

	float playerLookAngle = 0.f;
	float hitLookAngle = 0.f;

	float debugTimer = 0.f;
	float debugDuration = 1.f;

	float attackDistance = 100.f;

	float speed = 400.f;
	float dashDistance = 400.f;

	float dashDuration = 0.3f;
	float dashTimer = 0.f;

	// 슬라이딩 도중 방향전환 방지용
	float slideDuration = 0.3f;
	float slideTimer = 0.f;

	float dashCoolTimer = 0.f;
	float dashCoolDuration = 0.3f;

	float hitTimer = 0.f;
	float hitDuration = 0.3;



	// 상태변수
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


	std::vector<std::string> idleId;
	std::vector<std::string> runId;
	std::vector<std::string> dashId;
	std::vector<std::string> slideId;
	std::vector<std::string> attackId;
	std::vector<std::string> hitId;
	//std::vector<std::string> daedId;

	std::string attackName;

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
	void DieUpdate(float dt);

	void CalDir();
	void CalLookAngle();
	void CalHitLookAngle();



	void SetScene(SceneGame* scene) { this->scene = scene; }
	void SetMonster(Monster* monster) { this->monster = monster; }	// 하드 코딩용
	void SetAttackPos();
	void SetDirIconPos();
	void SetDirIconDir();


	Monster* GetMonster() { return monster; }

	// 데미지 : -, 회복 : +
	void SetHp(int value);
	void SetHitDir(sf::Vector2f dir) { this->hitLook = dir; };

	// 스킬 생성 지점
	sf::Vector2f GetAttackPos() { return attackPos; }
	float GetPlayerLookAngle() { return playerLookAngle; }

	bool IsAlive() { return isAlive; }
	bool IsAttack() { return isAttack; }
	
	void ChangeState(States state);

	// 애니메이션 이름 등록
	void InsertAnimId();
};