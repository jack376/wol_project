#include "stdafx.h"
#include "Player.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneGame.h"
#include "SceneMgr.h"

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

	//SCENE_MGR.GetCurrScene()->ScreenToUiPos(GetPosition());
	dirIcon = (SpriteGo*)scene->AddGo(new SpriteGo("graphics/Player/UI/PlayerMarker.png"));
	dirIcon->sprite.setScale(5, 5);
	dirIcon->sprite.setColor(sf::Color::Color(255, 255, 255, 100));
	dirIcon->SetPosition(GetPosition());
	dirIcon->SetOrigin(Origins::MC);
	dirIcon->sortLayer = 20;
	dirIcon->sortOrder = -1;


	// 콜라이더

	rect.setSize({65, 120});
	rect.setOutlineThickness(1.f);
	rect.setOutlineColor(sf::Color::Green);
	rect.setFillColor(sf::Color::Transparent);

	attackPosCol.setRadius(5.f);
	attackPosCol.setOutlineThickness(1.f);
	attackPosCol.setOutlineColor(sf::Color::Red);
	attackPosCol.setFillColor(sf::Color::Transparent);
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

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackBackDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackBackLeft.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackBackRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackBackUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackForeDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackForeLeft.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackForeRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/AttackForeUp.csv"));

	anim.SetTarget(&sprite);
	anim.Play("IdleDown");

	// 플레이어 리셋
	hp = maxHp;
	attackCount = 0;
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	// 방향아이콘 움직임 플레이어와 동기화
	SetDirIconPos();
	SetDirIconDir();
	


	// 콜라이더 플레이어 동기화
	rect.setPosition(position);

	// 마우스 방향 기준 각도
	CalLookAngle();


	// 디버그 타이머
	//debugTimer += dt;
	//if (debugTimer > debugDuration)
	//{
	//	std::cout << "X : " << look.x << std::endl;
	//	std::cout << "Y : " << look.y << std::endl;
	//	std::cout << "Player Look : " << playerLookAngle << std::endl;
	//	std::cout << "Attack Dir : " << (int)attackDir << std::endl;
	//	
	//	debugTimer = 0.f;
	//}

	SetAttackPos();

	dir = { INPUT_MGR.GetAxisRaw(Axis::Horizontal), INPUT_MGR.GetAxisRaw(Axis::Vertical) };


	// 입력에 따른 방향 설정
	CalDir();

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Left) && !isAttack && !isDash && !isSlide)
	{
		attackCount++;
		sEvent = SkillEvents::Left;
		ChangeState(States::Attack);
	}
	
	// 대쉬 쿨타임 계산
	if(isDashCool)
		dashCoolTimer += dt;

	if (dashCoolTimer > dashCoolDuration && isDashCool)
	{
		isDashCool = false;
		dashCoolTimer = 0.f;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isDashCool && !isSlide && !isAttack)
	{
		ChangeState(States::Dash);
	}

	float magnitude = Utils::Magnitude(dir);

	if (magnitude > 1.f)
	{
		dir /= magnitude;
	}


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
	window.draw(attackPosCol);
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
	if(attackCount % 2 != 0)
		attackName = "Fore";
	else
		attackName = "Back";

	//std::cout << attackCount << std::endl;
	std::cout << attackName << std::endl;

	if(!isAttack)
	{
		switch (attackDir)
		{
		case AttackDir::Up:
			anim.Play("Attack" + attackName + "Up");

			break;
		case AttackDir::Right:
			SetFlipX(false);
			anim.Play("Attack" + attackName + "Right");

			break;
		case AttackDir::Down:
			anim.Play("Attack" + attackName + "Down");

			break;
		case AttackDir::Left:
			SetFlipX(true);
			anim.Play("Attack" + attackName + "Right");

			break;
		}

		switch (sEvent)
		{
		case SkillEvents::Left:
			//mgr.
			// fireball.//
			break;
		case SkillEvents::Right:
			// fireball//
			break;
		case SkillEvents::Space:

			break;
		case SkillEvents::Q:

			break;
		}
	}

	isAttack = true;
	if (isAttack)
	{
		//attackRangeCol.setPosition(attackPosCol.getPosition());

	}
	if (anim.IsAnimEndFrame())
	{
		isRun = false;
		isAttack = false;
		ChangeState(States::Idle);
	}
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

void Player::CalLookAngle()
{
	playerLookAngle = Utils::Angle(look);

	if (playerLookAngle < 0)
	{
		playerLookAngle += 180 * 2;
	}

	// 각도에 따른 공격 방향 설정
	if ((playerLookAngle < 45 && playerLookAngle >= 0) ||
		(playerLookAngle < 360 && playerLookAngle >= 315))
	{
		attackDir = AttackDir::Right;
	}
	if (playerLookAngle < 135 && playerLookAngle >= 45)
	{
		attackDir = AttackDir::Down;
	}
	if (playerLookAngle < 225 && playerLookAngle >= 135)
	{
		attackDir = AttackDir::Left;
	}
	if (playerLookAngle < 315 && playerLookAngle >= 225)
	{
		attackDir = AttackDir::Up;
	}
}

void Player::SetAttackPos()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(GetPosition());

	look = Utils::Normalize(mousePos - playerScreenPos);

	attackPos = { look.x * attackDistance + GetPosition().x , 
				look.y * attackDistance + GetPosition().y };

	// 공격 지점 가시화
	attackPosCol.setPosition(attackPos);
}

void Player::SetDirIconPos()
{
	dirIcon->SetPosition(GetPosition().x, GetPosition().y + 80.f);
}

void Player::SetDirIconDir()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f iconScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(dirIcon->GetPosition());

	iconLook = Utils::Normalize(mousePos - iconScreenPos);
	float angle = Utils::Angle(iconLook) + 90;
	dirIcon->sprite.setRotation(angle);
}

void Player::SetHp(int value)
{
	if (hp <= 0)
	{
		// Dead 상태
		isAlive = false;
	}
	hp += value;
}

void Player::ChangeState(States state)
{
	currentState = state;
}