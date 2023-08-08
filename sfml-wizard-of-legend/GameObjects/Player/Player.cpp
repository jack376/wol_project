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

	// Reset인지 Init()인지 생각
	destPos.push_back({ dashDistance , -dashDistance });
	destPos.push_back({ -dashDistance , -dashDistance });
	destPos.push_back({ dashDistance , dashDistance });
	destPos.push_back({ -dashDistance , dashDistance });
	destPos.push_back({ 0 , -dashDistance });
	destPos.push_back({ dashDistance , 0 });
	destPos.push_back({ 0 , dashDistance });
	destPos.push_back({ -dashDistance , 0 });
}

void Player::Release()
{
	// 클리어를 해줘야 하는지 결정해야함
	destPos.clear();
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
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Dash/DashDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Dash/DashRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Dash/DashUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Slide/SlideDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Slide/SlideRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Slide/SlideUp.csv"));

	anim.SetTarget(&sprite);
	anim.Play("IdleDown");

}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);

	dir = { INPUT_MGR.GetAxisRaw(Axis::Horizontal), INPUT_MGR.GetAxisRaw(Axis::Vertical) };


	// 입력에 따른 방향 설정
	CalDir();
	
	// 대쉬 쿨타임 계산
	if(isDashCool)
		dashCoolTimer += dt;

	if (dashCoolTimer > dashCoolDuration && isDashCool)
	{
		isDashCool = false;
		dashCoolTimer = 0.f;
	}


	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isDashCool && !isSlide)
	{
		ChangeState(States::Dash);
	}


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
	
	case States::Slide:
		SlideUpdate(dt);
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
	case Dir::UpRight:
		anim.Play("IdleRight");
		break;
	case Dir::UpLeft:
		SetFlipX(true);
		anim.Play("IdleRight");
		break;
	case Dir::DownRight:
		anim.Play("IdleRight");
		break;
	case Dir::DownLeft:
		SetFlipX(true);
		anim.Play("IdleRight");
		break;
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
		anim.Play("RunDown");
	}

	if (dir.y < 0 && !isRun)
	{
		anim.Play("RunUp");
	}

	if (dir.x > 0 && !isRun)
	{
		anim.Play("RunRight");
		SetFlipX(false);
	}

	if (dir.x < 0 && !isRun)
	{
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
	if (!isDash)
	{
		switch (currentDir)
		{
		case Dir::UpRight:
			anim.Play("DashUp");

			break;
		case Dir::UpLeft:
			anim.Play("DashUp");

			break;
		case Dir::DownRight:
			anim.Play("DashDown");

			break;
		case Dir::DownLeft:
			anim.Play("DashDown");

			break;
		case Dir::Up:
			anim.Play("DashUp");

			break;
		case Dir::Right:
			anim.Play("DashRight");


			break;
		case Dir::Down:
			anim.Play("DashDown");

			break;
		case Dir::Left:
			SetFlipX(true);
			anim.Play("DashRight");

			break;
		}

		dashDir = dir;	// sf::Vector2f
		slideDir = currentDir;	// Dir,  슬라이딩 도중 방향전환 방지용
		dashDest = destPos[(int)currentDir] + GetPosition();
		dashStart = GetPosition();

	}
	isDash = true;

	if (isDash)
	{
		dashTimer += dt;
	}

	float t = Utils::Clamp(dashTimer / dashDuration, 0.f, 1.f);
	SetPosition(Utils::Lerp(dashStart, dashDest, t));

	if (t >= 1.0f)
	{
		dashTimer = 0.f;
		isDash = false;
		isRun = false;
		isSlide = false;
		isDashCool = true;
		ChangeState(States::Slide);
	}
}

void Player::SlideUpdate(float dt)
{
	if (!isSlide)
	{
		switch (slideDir)
		{
		case Dir::UpRight:
			anim.Play("SlideUp");

			break;
		case Dir::UpLeft:
			anim.Play("SlideUp");

			break;
		case Dir::DownRight:
			anim.Play("SlideDown");

			break;
		case Dir::DownLeft:
			anim.Play("SlideDown");

			break;
		case Dir::Up:
			anim.Play("SlideUp");

			break;
		case Dir::Right:
			anim.Play("SlideRight");


			break;
		case Dir::Down:
			anim.Play("SlideDown");

			break;
		case Dir::Left:
			SetFlipX(true);
			anim.Play("SlideRight");

			break;
		}
	}
	isSlide = true;

	if (isSlide)
		slideTimer += dt;

	if (anim.IsAnimEndFrame() && slideTimer > slideDuration)
	{
		slideTimer = 0.f;
		isDash = false;
		isRun = false;
		isSlide = false;
		ChangeState(States::Idle);
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

void Player::CalDir()
{
	if (dir.y < 0 && dir.x > 0)
	{
		currentDir = Dir::UpRight;
	}
	else if (dir.y < 0 && dir.x < 0)
	{
		currentDir = Dir::UpLeft;
	}
	else if (dir.y > 0 && dir.x > 0)
	{
		currentDir = Dir::DownRight;
	}
	else if (dir.y > 0 && dir.x < 0)
	{
		currentDir = Dir::DownLeft;
	}
	else if (dir.x > 0)
	{
		currentDir = Dir::Right;
	}
	else if (dir.x < 0)
	{
		currentDir = Dir::Left;
	}
	else if (dir.y > 0)
	{
		currentDir = Dir::Down;
	}
	else if (dir.y < 0)
	{
		currentDir = Dir::Up;
	}

}

void Player::SetHp(int value)
{
	hp -= value;
}

float Player::CalDashDistance(Dir dir)
{
	return 0;
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