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
	float speed = 400.f;
	float dashSpeed = 1700.f;
	float dashDistance = 400.f;

	// ���º���
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


	// ������ : -, ȸ�� : +
	void SetHp(int value);
	

	void ChangeState(States state);

	bool IsAlive() { return isAlive; }

	//void PlayerTextureReset();
};