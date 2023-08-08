#pragma once
#include "SpriteGo.h"
#include "AnimationController.h"

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

enum class States
{
	Idle,
	Run,
	Dash,
	Slide,
	Attack,
	KnockBack,
	Dead,
};

class Player : public SpriteGo
{
private:
	AnimationController anim;
	States currentState = States::Idle;
	Dir currentDir = Dir::Down;
	Dir slideDir = Dir::Down;

	int hp = 100;

	sf::Vector2f dir;
	sf::Vector2f dashDir;
	std::vector<sf::Vector2f> destPos;
	float speed = 400.f;
	float dashDistance = 400.f;

	float dashDuration = 0.3f;
	float dashTimer = 0.f;

	// 슬라이딩 도중 방향전환 방지용
	float slideDuration = 0.3f;
	float slideTimer = 0.f;

	float dashCoolTimer = 0.f;
	float dashCoolDuration = 0.3f;

	sf::Vector2f dashStart;
	sf::Vector2f dashDest;

	// 상태변수
	bool isAlive = true;
	bool isRun = false;
	bool isDash = false;
	bool isSlide = false;
	bool isDashCool = false;

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
	void KnockBackUpdate(float dt);
	void DeadUpdate(float dt);

	void CalDir();


	// 데미지 : -, 회복 : +
	void SetHp(int value);
	bool IsAlive() { return isAlive; }
	
	float CalDashDistance(Dir dir);

	void ChangeState(States state);


	//void PlayerTextureReset();
};