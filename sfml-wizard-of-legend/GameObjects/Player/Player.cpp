#include "stdafx.h"
#include "Player.h"
#include "ResourceMgr.h"
#include "InputMgr.h"

Player::Player(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

Player::~Player()
{
}

void Player::Init()
{
	SpriteGo::Init();
	SetOrigin(Origins::MC);
}

void Player::Release()
{
	SpriteGo::Release();

}

void Player::Reset()
{
	SpriteGo::Reset();
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Run/RunUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Run/RunRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Run/RunDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Idle/IdleDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Idle/IdleRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Idle/IdleUp.csv"));

	anim.SetTarget(&sprite);
	anim.Play("IdleDown");

}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	dir = { INPUT_MGR.GetAxisRaw(Axis::Horizontal), INPUT_MGR.GetAxisRaw(Axis::Vertical) };
	

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		ChangeState(States::Dash);
	}
	//std::cout << dir.x << std::endl;
	//std::cout << dir.y << std::endl;

	float magnitude = Utils::Magnitude(dir);

	if (magnitude > 1.f)
	{
		dir /= magnitude;
	}

	//ChangeState(States::Idle);
	//ChangeState(States::Dash);
	//ChangeState(States::Attack);
	//ChangeState(States::KnockBack);

	switch (currentState)
	{
	case States::Idle:
		IdleUpdate(dt);
		break;

	case States::Run:
		RunUpdate(dt);
		break;

	case States::Dash:
		DashUpdate(dt);
		break;

	case States::Attack:
		AttackUpdate(dt);
		break;

	case States::KnockBack:
		KnockBackUpdate(dt);
		break;

	case States::Dead:
		DeadUpdate(dt);
		break;
	}

	anim.Update(dt);
	SetOrigin(origin);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

}

void Player::IdleUpdate(float dt)
{
	switch (currentDir)
	{
	case Dir::Up:
		anim.Play("IdleUp");
		break;
	case Dir::Right:
		anim.Play("IdleRight");
		break;
	case Dir::Down:
		anim.Play("IdleDown");
		break;
	case Dir::Left:
		SetFlipX(true);
		anim.Play("IdleRight");
		break;
	}


	if (dir.x != 0 || dir.y != 0)
	{
		ChangeState(States::Run);
	}
}

void Player::RunUpdate(float dt)
{

	if (dir.y > 0 && !isRun)
	{
		currentDir = Dir::Down;
		anim.Play("RunDown");
	}

	if (dir.y < 0 && !isRun)
	{
		currentDir = Dir::Up;
		anim.Play("RunUp");
	}

	if (dir.x > 0 && !isRun)
	{
		currentDir = Dir::Right;
		anim.Play("RunRight");
		SetFlipX(false);
	}

	if (dir.x < 0 && !isRun)
	{
		currentDir = Dir::Left;
		anim.Play("RunRight");
		SetFlipX(true);
	}
	sf::Vector2f movePos = sprite.getPosition();
	movePos += dir * speed * dt;

	SetPosition(movePos);
	isRun = true;

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::W) ||
		INPUT_MGR.GetKeyDown(sf::Keyboard::A) || 
		INPUT_MGR.GetKeyDown(sf::Keyboard::S) || 
		INPUT_MGR.GetKeyDown(sf::Keyboard::D))
	{
		isRun = false;
	}


	if (dir.x == 0 && dir.y == 0)
	{
		isRun = false;
		ChangeState(States::Idle);
	}
}

void Player::DashUpdate(float dt)
{
	if (currentDir == Dir::Up)
	{
		float movePos = sprite.getPosition().y;
		//movePos += dir * dashSpeed * dt;
	}
	else if (currentDir == Dir::Right)
	{
		sf::Vector2f movePos = sprite.getPosition();
		movePos += dir * dashSpeed * dt;
	}
	else if (currentDir == Dir::Down)
	{
		sf::Vector2f movePos = sprite.getPosition();
		movePos += dir * dashSpeed * dt;
	}
	else if (currentDir == Dir::Left)
	{
		sf::Vector2f movePos = sprite.getPosition();
		movePos += dir * dashSpeed * dt;
	}
}

void Player::AttackUpdate(float dt)
{

}

void Player::KnockBackUpdate(float dt)
{

}

void Player::DeadUpdate(float dt)
{
}

void Player::ChangeState(States state)
{
	currentState = state;
}

//void Player::PlayerTextureReset()
//{
//	sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
//	if (tex != nullptr)
//	{
//		sprite.setTexture(*tex);
//	}
//	SetOrigin(origin);
//	SetPosition(GetPosition());
//
//}