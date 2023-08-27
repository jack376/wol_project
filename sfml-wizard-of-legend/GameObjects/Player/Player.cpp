#include "stdafx.h"
#include "Player.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "SkillMgr.h"
#include "Skill.h"
#include "Tile.h"
#include "SpriteEffect.h"

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
	sf::Vector2f normalize = Utils::Normalize({ 1, 1 });
	destPos.push_back({ dashDistance * normalize.x , -dashDistance * normalize.y });
	destPos.push_back({ -dashDistance * normalize.x, -dashDistance * normalize.y });
	destPos.push_back({ dashDistance * normalize.x , dashDistance * normalize.y });
	destPos.push_back({ -dashDistance * normalize.x, dashDistance * normalize.y });
	destPos.push_back({ 0 , -dashDistance });
	destPos.push_back({ dashDistance , 0 });
	destPos.push_back({ 0 , dashDistance });
	destPos.push_back({ -dashDistance , 0 });

	dirIcon = (SpriteGo*)scene->AddGo(new SpriteGo("graphics/Player/UI/PlayerMarker.png"));
	dirIcon->sprite.setScale(5, 5);
	dirIcon->sprite.setColor(sf::Color::Color(255, 255, 255, 100));
	dirIcon->SetPosition(GetPosition());
	dirIcon->SetOrigin(Origins::MC);
	dirIcon->sortLayer = 20;
	dirIcon->sortOrder = -1;

	portal = (SpriteGo*)scene->AddGo(new SpriteGo("graphics/Player/ExitPortal.png"));
	portal->sprite.setScale(4, 4);
	portal->SetOrigin(Origins::ML);
	portal->sortLayer = 20;
	portal->sortLayer = 0;
	portal->SetActive(false);


	// 콜라이더
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
	CalculatorCurrentTile();

	spellPool.OnCreate = [this](ElementalSpell* spell)
	{
		spell->SetMonsterList(monsters);
		spell->SetPool(spellPool);
	};

	playerDieEffectPool.OnCreate = [this](SpriteEffect* effect) {
		effect->sprite.setScale(4, 4);
		effect->SetAnimId("DieEffect");
		effect->SetDuration(0.5f);
		effect->sortLayer = 20;
	};
	spellPool.Init();
	playerDieEffectPool.Init();

	

	// Player Shader
	float yOffset = 15.0f / 31.0f;
	paletteTexture.loadFromFile("shader/WizardPalette.png");
	shader.loadFromFile("shader/FragShader.frag", sf::Shader::Fragment);
	shader.setUniform("paletteTexture", paletteTexture);
	shader.setUniform("paletteYOffset", yOffset);
}

void Player::Release()
{
	// 클리어를 해줘야 하는지 결정해야함
	for (auto obj : playerDieEffectPool.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(obj);
	}
	playerDieEffectPool.Clear();

	destPos.clear();
	SpriteGo::Release();
}

void Player::Reset()
{
	SpriteGo::Reset();
	playerDieEffectPool.Clear();

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

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Effects/HitEffect.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Die/Die.csv"));

	anim.SetTarget(&sprite);
	anim.Play("IdleDown");

	// 플레이어 리셋
	hp = maxHp;
	attackCount = 0;
	isAlive = true;
	currentDir = Dir::Down;

	portal->SetActive(false);
}

void Player::Update(float dt)
{
	SpriteGo::Update(dt);


	// 방향아이콘 움직임 플레이어와 동기화
	SetDirIconPos();
	SetDirIconDir();
	
	// 디버그 위치


	// 콜라이더 플레이어 동기화
	rect.setPosition(position);

	// 마우스 방향 기준 각도
	CalLookAngle();
	SetAttackPos();



	dir = { INPUT_MGR.GetAxisRaw(Axis::Horizontal), INPUT_MGR.GetAxisRaw(Axis::Vertical) };
	// 입력에 따른 방향 설정
	CalDir();
	prevPos = GetPosition();

	isMove = dir.x != 0 || dir.y != 0;
	isDashing = dashDir.x != 0 || dashDir.y != 0;


	if (!isAlive)
	{
		ChangeState(States::Die);
	}

	// 무적 상태 분리
	if (isInvincible)
	{

	}

	// 맞으면 무적상태와 맞는 상태 표시
	if (isHit && isAlive && !isFalling)
	{
		scene->SetIsMenuOn(false);
		CalHitLookAngle();
		ChangeState(States::Hit);
	}

	if (scene->GetIsMenuOn() || scene->GetIsGameEnd())
		return;

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Left) && !isAttack && !isDash && !isSlide && !isFalling && !isHit && isAlive)
	{
		attackCount++;
		sEvent = SkillEvents::Left;
		if (!SKILL_MGR.SearchSkill(sEvent)->GetIsUsed())
		{
			SKILL_MGR.UseSkill(sEvent);
			ChangeState(States::Attack);
		}
	}

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Right) && !isAttack && !isDash && !isSlide && !isFalling && !isHit && isAlive)
	{
		attackCount++;
		sEvent = SkillEvents::Right;
		if (!SKILL_MGR.SearchSkill(sEvent)->GetIsUsed())
		{
			SKILL_MGR.UseSkill(sEvent);
			ChangeState(States::Attack);
		}
	}
	

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Q) && !isAttack && !isDash && !isSlide && !isFalling && !isHit && isAlive)
	{
		attackCount++;
		sEvent = SkillEvents::Q;
		if (!SKILL_MGR.SearchSkill(sEvent)->GetIsUsed())
		{
			SKILL_MGR.UseSkill(sEvent);
			ChangeState(States::Attack);
		}
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::E) && !isAttack && !isDash && !isSlide && !isFalling && !isHit && isAlive)
	{
		attackCount++;
		sEvent = SkillEvents::E;
		if (!SKILL_MGR.SearchSkill(sEvent)->GetIsUsed())
		{
			SKILL_MGR.UseSkill(sEvent);
			ChangeState(States::Attack);
		}
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::R) && !isAttack && !isDash && !isSlide && !isFalling && !isHit && isAlive)
	{
		attackCount++;
		sEvent = SkillEvents::R;
		if (!SKILL_MGR.SearchSkill(sEvent)->GetIsUsed())
		{
			SKILL_MGR.UseSkill(sEvent);
			ChangeState(States::Attack);
		}
	}

	// 대쉬 쿨타임 계산
	if(isDashCool)
		dashCoolTimer += dt;

	if (dashCoolTimer > dashCoolDuration && isDashCool)
	{
		isDashCool = false;
		dashCoolTimer = 0.f;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isDashCool && !isSlide && !isAttack && !isFalling && !isHit && isAlive)
	{
		sEvent = SkillEvents::Space;
		isAttack = true;
		SKILL_MGR.UseSkill(sEvent);
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

	case States::Fall:
		FallUpdate(dt);
		break;
	
	case States::Die:
		DieUpdate(dt);
		break;
	}
	
	anim.Update(dt);
	SetOrigin(origin);
	CalculatorCurrentTile();
	PortalAnimations(dt);
}

void Player::Draw(sf::RenderWindow& window)
{
	window.draw(sprite, &shader);
	window.draw(rect);
	window.draw(attackPosCol);
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
	CalculatorCurrentTile();

	// 타일 충돌 방지
	if ((currentTile->GetType() == TileType::Cliff ||
		currentTile->GetType() == TileType::Wall) && isMove)
	{
		SetPosition(prevPos);
	}

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
	CalculatorCurrentTile();

	// 타일 충돌 방지
	if(currentTile->GetType() == TileType::Wall)
	{
		isAttack = false;
		SetPosition(prevPos);
		ChangeState(States::Slide);
	}


	if (t >= 1.0f)
	{
		dashTimer = 0.f;
		isDash = false;
		isRun = false;
		isSlide = false;
		isAttack = false;
		isDashCool = true;

		if (currentTile->GetType() == TileType::Cliff)
		{
			//std::cout << "Cliff" << std::endl;
			ChangeState(States::Fall);
		}
		else
		{
			ChangeState(States::Slide);
		}
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
	}
	if (!isAttack)
	{
		sf::Vector2f hitDir = Utils::Angle(playerLookAngle);
		attackStart = GetPosition();
		attackDest = GetPosition() + sf::Vector2f{ hitDir.x * 50, hitDir.y * 50 };
	}

	isAttack = true;

	if (isAttack)
		attackMoveTimer += dt;

	float t = Utils::Clamp(attackMoveTimer / attackMoveDuration, 0.f, 1.f);
	SetPosition(Utils::Lerp(attackStart, attackDest, t));
	CalculatorCurrentTile();

	if (currentTile->GetType() == TileType::Wall || currentTile->GetType() == TileType::Cliff)
	{
		SetPosition(prevPos);
	}

	if (anim.IsAnimEndFrame() && attackMoveTimer > attackMoveDuration)
	{
		attackMoveTimer = 0.f;
		isRun = false;
		isAttack = false;
		ChangeState(States::Idle);
	}
}

void Player::HitUpdate(float dt)
{
	if (currentTile->GetType() == TileType::Wall)
		SetPosition(prevPos);

	currentDir = (Dir)((int)hitDir + 4);

	// 몬스터의 공격과 동기화가 되지 않음
	if (!isHitAnim)
	{
		anim.Play(hitId[(int)hitDir]);
		sprite.setColor(sf::Color::Red);
		isHitAnim = true;
	}

	//애니메이션이 너무 한 프레임에 처리되어서 시간초를 둠
	if (isHitAnim)
	{
		hitTimer += dt;
		sf::Vector2f hitKnockBackDir = Utils::Angle(hitLookAngle - 180);
		knockBackStart = GetPosition();
		knockBackDest = GetPosition() + sf::Vector2f{ hitKnockBackDir.x * 5, hitKnockBackDir.y * 5 };
	}

	if (hitTimer > hitDuration)
	{
		hitTimer = 0.f;
		isHitAnim = false;
	}

	float t = Utils::Clamp(hitTimer / hitDuration, 0.f, 1.f);
	SetPosition(Utils::Lerp(knockBackStart, knockBackDest, t));
	CalculatorCurrentTile();

	if (currentTile->GetType() == TileType::Wall)
	{
		SetPosition(prevPos);
	}

	switch (hitDir)
	{
	case HitDir::Up:

		//std::cout << "Hit Up" << std::endl;
		break;
	case HitDir::Right:
		//std::cout << "Hit Right" << std::endl;

		break;
	case HitDir::Down:
		//std::cout << "Hit Down" << std::endl;

		break;
	case HitDir::Left:
		SetFlipX(true);
		//std::cout << "Hit Left" << std::endl;
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

void Player::FallUpdate(float dt)
{
	if (!isFalling)
	{
		anim.Play(fallId[(int)slideDir]);
		isFalling = true;

		switch (slideDir)
		{
		case Dir::Left:
			SetFlipX(true);
			break;
		}

		fallStart = GetPosition();
		fallDest = GetPosition() + sf::Vector2f(0, 200.f);
	}

	if (isFalling)
	{
		fallTimer += dt;
	}

	float t = Utils::Clamp(fallTimer / fallDuration, 0.f, 1.f);
	SetPosition(Utils::Lerp(fallStart, fallDest, t));

	// 다 떨어지면
	if (t >= 1.0f && !isFallHit)
	{
		SetHp(-25);
		isFallHit = true;
		originAngle = sprite.getRotation();
		float randomAngle = Utils::RandomRange(0.f, 360.f);
		anim.Play("HitEffect");
		sprite.setColor(sf::Color::Red);
		sprite.setRotation(randomAngle);
	}

	if (isFallHit)
	{
		fallHitTimer += dt;
	}

	// 맞는 애니메이션 실행 끝나면
	if (fallHitTimer > fallHitDuration)
	{
		// 제자리 돌아가기
		sprite.setRotation(originAngle);
		isFallHit = false;
		isFalling = false;
		fallTimer = 0.f;
		fallHitTimer = 0.f;
		SetPosition(dashStart);
		portal->SetActive(true);
		portal->SetPosition(GetPosition().x, GetPosition().y);
		portal->SetOrigin(Origins::MC);
		isPortalBigAnim = true;
		ChangeState(States::Idle);
	}

}

void Player::DieUpdate(float dt)
{
	// 죽을떄 시간 흐름 느리게 하기 생각
	// 빨개졌다가 다시 하얘짐
	if (!isDieAnim)
	{
		anim.Play("Die");

		isDieAnim = true;
	}

	if (anim.GetCurrentClipId() == "Die")
	{
		anim.GetCurrentClip().frames[1].action = [this]() {
			anim.SetSpeed(0.1f);
			SpriteEffect* playerHitEffect = playerDieEffectPool.Get();
			sf::Vector2f randPos(Utils::RandomRange(0.f, 1.f), Utils::RandomRange(0.f, 1.f));
			playerHitEffect->SetPosition(GetPosition()+ randPos);
			float randAngle = Utils::RandomRange(0.f, 360.f);
			playerHitEffect->sprite.setRotation(randAngle);
			playerHitEffect->sprite.setColor(sf::Color::Red);
			SCENE_MGR.GetCurrScene()->AddGo(playerHitEffect);

		};
	
		anim.GetCurrentClip().frames[3].action = [this]() {
			anim.SetSpeed(0.8f);

		};
		anim.GetCurrentClip().frames[5].action = [this]() {
			anim.SetSpeed(1.0f);

		};
	}


	if (anim.IsAnimEndFrame())
	{
		scene->SetIsGameEnd(true);
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

void Player::CalHitLookAngle()
{
	hitLookAngle = Utils::Angle(hitLook);

	if (hitLookAngle < 0)
	{
		hitLookAngle += 180 * 2;
	}

	hitLookAngle += 180;

	// 때린 방향 반대로 전환
	if (hitLookAngle >= 360)
	{
		//hitLookAngle -= 360;
		hitLookAngle = (float)((int)hitLookAngle % 360);
	}


	// 각도에 따른 공격 방향 설정
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
	isHit = true;
	//isInvincible = true;
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

	attackId.push_back("AttackUp");
	attackId.push_back("AttackRight");
	attackId.push_back("AttackDown");
	attackId.push_back("AttackRight");

	hitId.push_back("HitUp");
	hitId.push_back("HitRight");
	hitId.push_back("HitDown");
	hitId.push_back("HitRight");

	fallId.push_back("HitUp");
	fallId.push_back("HitUp");
	fallId.push_back("HitDown");
	fallId.push_back("HitDown");
	fallId.push_back("HitUp");
	fallId.push_back("HitRight");
	fallId.push_back("HitDown");
	fallId.push_back("HitRight");
}

void Player::CalculatorCurrentTile()
{
	int rowIndex = position.x < 0 ? 0 : position.x / _TileSize;
	int columnIndex = position.y < 0 ? 0 : position.y / _TileSize;

 	currentTile = (*worldTiles)[rowIndex][columnIndex];
}

std::vector<Tile*> Player::CalculatorRangeTiles(int row, int col)
{
	//32x16
	int searchRowRange = row;
	int searchColRange = col;

	sf::Vector2i index = currentTile->GetIndex();
	std::vector<Tile*> tiles;

	int topRowIndex = index.x - searchRowRange < 0 ? 0 : index.x > worldTiles->size() * _TileSize ? worldTiles->size() * _TileSize : index.x;
	int leftColumnIndex = index.y - searchColRange < 0 ? 0 : index.y > worldTiles[0].size() * _TileSize ? worldTiles[0].size() * _TileSize : index.y;
	for (int i = topRowIndex; i < index.x + searchRowRange; i++)
	{
		for (int j = leftColumnIndex; j < index.y + searchColRange; j++)
		{
			tiles.push_back((*this->worldTiles)[i][j]);
		}
	}

	return tiles;
}

void Player::PortalAnimations(float dt)
{
	if (isPortalBigAnim)
		portalTimer += dt;

	if (isPortalAnimTerm)
		portalTermTimer += dt;


	if (isPortalSmallAnim)
		portalTimer -= dt;


	float t = Utils::Clamp(portalTimer / portalDuration, 0.f, 1.f);

	if (t >= 1.0f && isPortalBigAnim)
	{
		isPortalBigAnim = false;
		isPortalAnimTerm = true;
	}


	if (portalTermTimer > portalDuration)
	{
		isPortalAnimTerm = false;
		isPortalSmallAnim = true;
	}

	if (t <= 0.f && isPortalSmallAnim)
	{
		portalTimer = 0.f;
		portalTermTimer = 0.f;
		isPortalSmallAnim = false;
	}


	if (isPortalBigAnim)
	{
		portal->sprite.setScale(Utils::Lerp(1.f, 4.f, t), Utils::Lerp(2.f, 4.f, t));
	}

	if (isPortalSmallAnim)
	{
		portal->sprite.setScale(Utils::Lerp(0.f, 4.f, t), Utils::Lerp(0.f, 4.f, t));
	}
	portal->SetOrigin(Origins::MC);
}
