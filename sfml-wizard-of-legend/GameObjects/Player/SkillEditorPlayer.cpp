#include "stdafx.h"
#include "SkillEditorPlayer.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "SkillMgr.h"
#include "Skill.h"
#include "Tile.h"

SkillEditorPlayer::SkillEditorPlayer(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

SkillEditorPlayer::~SkillEditorPlayer()
{
}

void SkillEditorPlayer::Init()
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




	// 콜라이더
	rect.setSize({ 65, 120 });
	rect.setOutlineThickness(1.f);
	rect.setOutlineColor(sf::Color::Green);
	rect.setFillColor(sf::Color::Transparent);

	attackPosCol.setRadius(5.f);
	attackPosCol.setOutlineThickness(1.f);
	attackPosCol.setOutlineColor(sf::Color::Red);
	attackPosCol.setFillColor(sf::Color::Transparent);

	InsertAnimId();
	playerColor = sprite.getColor();

	spellPool.OnCreate = [this](SkillEditorElementalSpell* spell)
	{
		spell->SetPool(spellPool);
	};

	//sf::Image grayImage = sprite.getTexture()->copyToImage();
	//sf::Vector2u imageSize = grayImage.getSize();
}

void SkillEditorPlayer::Release()
{
	// 클리어를 해줘야 하는지 결정해야함
	destPos.clear();
	SpriteGo::Release();
}

void SkillEditorPlayer::Reset()
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

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Effects/HitEffect.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Die/Die.csv"));

	anim.SetTarget(&sprite);
	anim.Play("IdleDown");

	// 플레이어 리셋
	hp = maxHp;
	attackCount = 0;
	dirIcon = (SpriteGo*)SCENE_MGR.GetCurrScene()->AddGo(new SpriteGo("graphics/Player/UI/PlayerMarker.png"));
	dirIcon->sprite.setScale(5, 5);
	dirIcon->sprite.setColor(sf::Color::Color(255, 255, 255, 100));
	dirIcon->SetPosition(GetPosition());
	dirIcon->SetOrigin(Origins::MC);
	dirIcon->sortLayer = 20;
	dirIcon->sortOrder = -1;

	spellPool.Init();


	// 팔레트 적용시키기
	//palette.setTexture(*RESOURCE_MGR.GetTexture("graphics/Player/WizardPalette.png"));
	//palette.setTextureRect(sf::IntRect{0, 62, 54, 2});
	////palette.setScale(16, 16);

	//sf::Image grayImage = sprite.getTexture()->copyToImage();
	//sf::Image paletteImage = palette.getTexture()->copyToImage();
	//paletteTexture.loadFromImage(grayImage);

	//sprite = Utils::SetPixelColor(grayImage, paletteImage);

	// 얽히고 섥혀서 문제 발생 여지 있음..
	// &로 넘기긴 하는데 적용이 되는건지 확인 필요
	//Utils::SetShader(currentShader, sprite, paletteTexture);

	//sf::Texture* tex = RESOURCE_MGR.GetTexture(frame.textureId);

	////여기서 target의 texture와 Rect를 정함!
	//target->setTexture(*tex);
	//target->setTextureRect(frame.texCoord);

}

void SkillEditorPlayer::Update(float dt)
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


	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Left) && !isAttack && !isDash && !isSlide)
	{
		attackCount++;
		sEvent = SkillEvents::Left;

		//ElementalSpell* elemental = spellPool.Get();
		//elemental->SetPlayer(this);
		//elemental->sortLayer = 21;
		//elemental->SetSkillType(SkillTypes::Melee);
		//elemental->SetTiles(wouldTiles);
		//elemental->SetMonsterList(monsters);
		//scene->AddGo(elemental);

		//elemental->SetScene(scene);

		//SKILL_MGR.UseEditorSkill(sEvent);

		ChangeState(States::Attack);

	}

	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Button::Right) && !isAttack && !isDash && !isSlide)
	{
		attackCount++;
		sEvent = SkillEvents::Right;

		//SKILL_MGR.UseEditorSkill(sEvent);
		//ElementalSpell* elemental = spellPool.Get();
		//elemental->SetScene(scene);
		//elemental->SetPlayer(this);
		//elemental->sortLayer = 21;
		//elemental->SetSkillType(SkillTypes::Range);
		//elemental->SetRangeType(RangeTypes::Curve);
		//elemental->SetTiles(wouldTiles);
		//elemental->SetMonsterList(monsters);
		//scene->AddGo(elemental);

		ChangeState(States::Attack);
	}


	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Q) && !isAttack && !isDash && !isSlide )
	{
		sEvent = SkillEvents::Q;
		//SKILL_MGR.UseEditorSkill(sEvent);
	}

	// 대쉬 쿨타임 계산
	if (isDashCool)
		dashCoolTimer += dt;

	if (dashCoolTimer > dashCoolDuration && isDashCool)
	{
		isDashCool = false;
		dashCoolTimer = 0.f;
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && !isDashCool && !isSlide && !isAttack )
	{
		sEvent = SkillEvents::Space;
		ChangeState(States::Dash);
		//SKILL_MGR.UseSkill(sEvent);
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

	}

	anim.Update(dt);
	SetOrigin(origin);
}

void SkillEditorPlayer::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(palette);
	window.draw(attackPosCol);
	//window.draw(sprite, &currentShader);
}

void SkillEditorPlayer::IdleUpdate(float dt)
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

void SkillEditorPlayer::RunUpdate(float dt)
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

void SkillEditorPlayer::DashUpdate(float dt)
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

void SkillEditorPlayer::SlideUpdate(float dt)
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

void SkillEditorPlayer::AttackUpdate(float dt)
{
	if (attackCount % 2 != 0)
		attackName = "Fore";
	else
		attackName = "Back";

	if (!isAttack)
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

void SkillEditorPlayer::CalDir()
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

void SkillEditorPlayer::CalLookAngle()
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

void SkillEditorPlayer::SetAttackPos()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f playerScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(GetPosition());

	look = Utils::Normalize(mousePos - playerScreenPos);

	attackPos = { look.x * attackDistance + GetPosition().x ,
				look.y * attackDistance + GetPosition().y };

	// 공격 지점 가시화
	attackPosCol.setPosition(attackPos);
}

void SkillEditorPlayer::SetDirIconPos()
{
	dirIcon->SetPosition(GetPosition().x, GetPosition().y + 80.f);
}

void SkillEditorPlayer::SetDirIconDir()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f iconScreenPos = SCENE_MGR.GetCurrScene()->WorldPosToScreen(dirIcon->GetPosition());

	iconLook = Utils::Normalize(mousePos - iconScreenPos);
	float angle = Utils::Angle(iconLook) + 90;
	dirIcon->sprite.setRotation(angle);
}

void SkillEditorPlayer::ChangeState(States state)
{
	currentState = state;
}

void SkillEditorPlayer::InsertAnimId()
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

	// 중간에 attackName Insert하기
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