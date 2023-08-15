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

	// Reset���� Init()���� ����
	sf::Vector2f normalize = Utils::Normalize({ 1, 1 });
	destPos.push_back({ dashDistance * normalize.x , -dashDistance * normalize.y });
	destPos.push_back({ -dashDistance * normalize.x, -dashDistance * normalize.y });
	destPos.push_back({ dashDistance * normalize.x , dashDistance * normalize.y });
	destPos.push_back({ -dashDistance * normalize.x, dashDistance * normalize.y });
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


	// �ݶ��̴�
	rect.setSize({65, 120});
	rect.setOutlineThickness(1.f);
	rect.setOutlineColor(sf::Color::Green);
	rect.setFillColor(sf::Color::Transparent);

	attackPosCol.setRadius(5.f);
	attackPosCol.setOutlineThickness(1.f);
	attackPosCol.setOutlineColor(sf::Color::Red);
	attackPosCol.setFillColor(sf::Color::Transparent);

	InsertAnimId();
	playerColor = sprite.getColor();

	//sf::Image grayImage = sprite.getTexture()->copyToImage();
	//sf::Vector2u imageSize = grayImage.getSize();
}

void Player::Release()
{
	// Ŭ��� ����� �ϴ��� �����ؾ���
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

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackBackDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackBackLeft.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackBackRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackBackUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackForeDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackForeLeft.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackForeRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Hand/AttackForeUp.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Kick/KickUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Kick/KickRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Kick/KickDown.csv"));	
	
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Jump/JumpUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Jump/JumpRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Jump/JumpDown.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Slam/SlamUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Slam/SlamDown.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Focus/FocusUp.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Focus/FocusRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attack/Focus/FocusDown.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Hit/HitDown.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Hit/HitRight.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Hit/HitUp.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Die/Die.csv"));

	anim.SetTarget(&sprite);
	anim.Play("IdleDown");

	// �÷��̾� ����
	hp = maxHp;
	attackCount = 0;

	// �ȷ�Ʈ �����Ű��
	//palette.setTexture(*RESOURCE_MGR.GetTexture("graphics/Player/WizardPalette.png"));
	//palette.setTextureRect(sf::IntRect{0, 62, 54, 2});
	////palette.setScale(16, 16);

	//sf::Image grayImage = sprite.getTexture()->copyToImage();
	//sf::Image paletteImage = palette.getTexture()->copyToImage();
	//paletteTexture.loadFromImage(grayImage);

	//sprite = Utils::SetPixelColor(grayImage, paletteImage);

	// ������ ������ ���� �߻� ���� ����..
	// &�� �ѱ�� �ϴµ� ������ �Ǵ°��� Ȯ�� �ʿ�
	//Utils::SetShader(currentShader, sprite, paletteTexture);

	//sf::Texture* tex = RESOURCE_MGR.GetTexture(frame.textureId);

	////���⼭ target�� texture�� Rect�� ����!
	//target->setTexture(*tex);
	//target->setTextureRect(frame.texCoord);

}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);
	// ��������� ������ �÷��̾�� ����ȭ
	SetDirIconPos();
	SetDirIconDir();
	
	// ����� ��ġ


	// �ݶ��̴� �÷��̾� ����ȭ
	rect.setPosition(position);

	// ���콺 ���� ���� ����
	CalLookAngle();


	// ����� Ÿ�̸�
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


	if (!isAlive)
	{
		ChangeState(States::Die);
	}

	// ���� ���� �и�
	if (isInvincible)
	{

	}

	// ������ �������¿� �´� ���� ǥ��
	if (isHit)
	{
		CalHitLookAngle();
		ChangeState(States::Hit);
	}

	// �Է¿� ���� ���� ����
	CalDir();

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Left) && !isAttack && !isDash && !isSlide)
	{
		attackCount++;
		sEvent = SkillEvents::Left;
		ChangeState(States::Attack);
	}
	
	// �뽬 ��Ÿ�� ���
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

	case States::Hit:
		HitUpdate(dt);
		break;

	case States::Die:
		DieUpdate(dt);
		break;
	}

	anim.Update(dt);
	SetOrigin(origin);
}

void Player::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(palette);
	window.draw(attackPosCol);
	//window.draw(sprite, &currentShader);
}

void Player::IdleUpdate(float dt)
{
	sprite.setColor(playerColor);
	anim.Play(idleId[(int)currentDir]);
	switch (currentDir)
	{
	case Dir::UpLeft:
	case Dir::DownLeft:
	case Dir::Left:
		SetFlipX(true);
		break;
	}	
	
	if (dir.x != 0 || dir.y != 0)
	{
		ChangeState(States::Run);
	}
}

void Player::RunUpdate(float dt)
{
	if (!isRun)
	{
		anim.Play(runId[(int)currentDir]);

		switch (currentDir)
		{
		case Dir::UpLeft:
		case Dir::DownLeft:
		case Dir::Left:
			SetFlipX(true);
			break;
		default:
			SetFlipX(false);

		}

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
		anim.Play(dashId[(int)currentDir]);

		switch (currentDir)
		{

		case Dir::Left:
			SetFlipX(true);
			break;
		}

		dashDir = dir;	// sf::Vector2f
		slideDir = currentDir;	// Dir,  �����̵� ���� ������ȯ ������
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
		anim.Play(slideId[(int)slideDir]);

		switch (slideDir)
		{
		case Dir::Left:
			SetFlipX(true);
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

	if(!isAttack)
	{
		std::string resultId = attackId[(int)attackDir];
		resultId.insert(attackNameInsertPos, attackName);
		anim.Play(resultId);

		currentDir = (Dir)((int)attackDir + 4);

		switch (attackDir)
		{
		case AttackDir::Right:
			SetFlipX(false);
			break;
		case AttackDir::Left:
			SetFlipX(true);
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
	if (anim.IsAnimEndFrame())
	{
		isRun = false;
		isAttack = false;
		ChangeState(States::Idle);
	}
}

void Player::HitUpdate(float dt)
{

	currentDir = (Dir)((int)hitDir + 4);

	// ������ ���ݰ� ����ȭ�� ���� ����
	if (!isHitAnim)
	{
		anim.Play(hitId[(int)hitDir]);
		sprite.setColor(sf::Color::Red);
		isHitAnim = true;
	}

	//�ִϸ��̼��� �ʹ� �� �����ӿ� ó���Ǿ �ð��ʸ� ��
	if (isHitAnim)
	{
		hitTimer += dt;
	}

	if (hitTimer > hitDuration)
	{
		hitTimer = 0.f;
		isHitAnim = false;
	}

	// ������ ���� �����̱⿡
	// ������ ���ϰ� �ٸ� ���⿡�� �¾Ƶ� �װ� ����
	switch (hitDir)
	{
	case HitDir::Up:

		std::cout << "Hit Up" << std::endl;
		break;
	case HitDir::Right:
		std::cout << "Hit Right" << std::endl;

		break;
	case HitDir::Down:
		std::cout << "Hit Down" << std::endl;

		break;
	case HitDir::Left:
		SetFlipX(true);
		std::cout << "Hit Left" << std::endl;
		break;
	}

	if (anim.IsAnimEndFrame() && !isHitAnim)
	{
		sprite.setColor(playerColor);
		isHit = false;
		isHitAnim = false;
		isRun = false;
		isAttack = false;
		ChangeState(States::Idle);
	}

}

void Player::DieUpdate(float dt)
{
	// ������ �ð� �帧 ������ �ϱ� ����
	// �������ٰ� �ٽ� �Ͼ���
	if (!isDieAnim)
	{
		anim.Play("Die");
		isDieAnim = true;
	}
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

	// ������ ���� ���� ���� ����
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

void Player::CalHitLookAngle()
{
	hitLookAngle = Utils::Angle(hitLook);

	if (hitLookAngle < 0)
	{
		hitLookAngle += 180 * 2;
	}

	hitLookAngle += 180;

	// ���� ���� �ݴ�� ��ȯ
	if (hitLookAngle >= 360)
	{
		//hitLookAngle -= 360;
		hitLookAngle = (float)((int)hitLookAngle % 360);
	}


	// ������ ���� ���� ���� ����
	if ((hitLookAngle < 45 && hitLookAngle >= 0) ||
		(hitLookAngle < 360 && hitLookAngle >= 315))
	{
		hitDir = HitDir::Right;
	}
	if (hitLookAngle < 135 && hitLookAngle >= 45)
	{
		hitDir = HitDir::Down;
	}
	if (hitLookAngle < 225 && hitLookAngle >= 135)
	{
		hitDir = HitDir::Left;
	}
	if (hitLookAngle < 315 && hitLookAngle >= 225)
	{
		hitDir = HitDir::Up;
	}

}

void Player::SetAttackPos()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(GetPosition());

	look = Utils::Normalize(mousePos - playerScreenPos);

	attackPos = { look.x * attackDistance + GetPosition().x , 
				look.y * attackDistance + GetPosition().y };

	// ���� ���� ����ȭ
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
	isHit = true;
	//isInvincible = true;
	if (hp <= 0)
	{
		// Dead ����
		isAlive = false;
	}
	hp += value;
}

void Player::ChangeState(States state)
{
	currentState = state;
}

void Player::InsertAnimId()
{
	idleId.push_back("IdleRight");
	idleId.push_back("IdleRight");
	idleId.push_back("IdleRight");
	idleId.push_back("IdleRight");
	idleId.push_back("IdleUp");
	idleId.push_back("IdleRight");
	idleId.push_back("IdleDown");
	idleId.push_back("IdleRight");


	runId.push_back("RunRight");
	runId.push_back("RunRight");
	runId.push_back("RunRight");
	runId.push_back("RunRight");
	runId.push_back("RunUp");
	runId.push_back("RunRight");
	runId.push_back("RunDown");
	runId.push_back("RunRight");

	dashId.push_back("DashUp");
	dashId.push_back("DashUp");
	dashId.push_back("DashDown");
	dashId.push_back("DashDown");
	dashId.push_back("DashUp");
	dashId.push_back("DashRight");
	dashId.push_back("DashDown");
	dashId.push_back("DashRight");

	slideId.push_back("SlideUp");
	slideId.push_back("SlideUp");
	slideId.push_back("SlideDown");
	slideId.push_back("SlideDown");
	slideId.push_back("SlideUp");
	slideId.push_back("SlideRight");
	slideId.push_back("SlideDown");
	slideId.push_back("SlideRight");

	// �߰��� attackName Insert�ϱ�
	attackId.push_back("AttackUp");
	attackId.push_back("AttackRight");
	attackId.push_back("AttackDown");
	attackId.push_back("AttackRight");

	hitId.push_back("HitUp");
	hitId.push_back("HitRight");
	hitId.push_back("HitDown");
	hitId.push_back("HitRight");
}
