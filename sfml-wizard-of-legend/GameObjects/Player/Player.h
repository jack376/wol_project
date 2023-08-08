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

	int hp = 100;

	sf::Vector2f dir;
	std::vector<sf::Vector2f> destPos;
	float speed = 400.f;
	float dashSpeed = 1700.f;
	float dashDistance = 400.f;

	// 상태변수
	bool isAlive = true;
	bool isRun = false;
	bool isDash = false;

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