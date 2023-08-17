#include "stdafx.h"
#include "ElementalSpell.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "Framework.h"
#include "BoxCollider2D.h"
#include "Tile.h"

ElementalSpell::ElementalSpell(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

ElementalSpell::~ElementalSpell()
{
}

void ElementalSpell::Init()
{
	Collider = (BoxCollider2D*)scene->AddGo(new BoxCollider2D());
}

void ElementalSpell::Release()
{
}

void ElementalSpell::Reset()
{
	SpriteGo::Reset();

	// 파일로 받아서 실행하는 방법 생각하기
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashLarge.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashMed.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashSmall.csv"));
	
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/FireBall/FireBall.csv"));

	sprite.setScale(2, 2);
	anim.SetTarget(&sprite);
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

	Collider->SetSprite(sprite);
	Collider->SetColSize();

	raycaster.move(position.x, position.y);
	raycaster.checkCollision(monster);

	// 이걸 플레이어에서 받는게 아니라 스킬에서 받는다.
	currentEvent = player->GetSkillEvent();
	switch(currentEvent)
	{
	case SkillEvents::Left:
		currentSkillType = SkillTypes::Melee;
		break;
	case SkillEvents::Right:
		currentSkillType = SkillTypes::Range;
		break;
	case SkillEvents::Q:
		break;
	case SkillEvents::Space:
		break;
	}


	switch (currentSkillType)
	{
	case SkillTypes::Melee:
		angle = player->GetPlayerLookAngle() + 90;
		MeleeUpdate(dt);
		break;

	case SkillTypes::Range:
		angle = player->GetPlayerLookAngle();
		RangeUpdate(dt);
		break;
	}



	anim.Update(dt);
	SetOrigin(Origins::MC);
	Collider->SetOrigin(Origins::MC);
	//CalculatorCurrentTile();
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	raycaster.draw(window);
}

void ElementalSpell::MeleeUpdate(float dt)
{
	// isCol은 콜라이더끼리 부딪혀야 true
	// player->IsAttack() 가 진짜 공격 타이밍 

	// 공격 스윙
	if (player->IsAttack() && !isSpawn)
	{
		isSpawn = true;
		Collider->SetActive(true);
		Collider->SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider->GetObbCol().setRotation(angle);
		comboQueue.push(FRAMEWORK.GetGamePlayTime());
	}

	// 콤보 넣는 구간
	if (!comboQueue.empty())
	{
		if (comboQueue.front() - prevComboTime > comboDuration)
		{
			attackCount = 0;
			//std::cout << "combo is Failed" << std::endl;
		}

		if (comboQueue.front() - prevComboTime < comboDuration)
		{
			attackCount++;
			//std::cout << "combo is Succesed" << std::endl;
		}

		// 인덱스를 이용해서 벡터 컨테이너 값 참조후 해당 애니메이션 실행하는 방법 생각
		if (attackCount % 3 == 0)
		{
			anim.Play("WindSlashSmall");
		}
		if (attackCount % 3 == 1)
		{
			anim.Play("WindSlashMed");
		}
		if (attackCount % 3 == 2)
		{
			anim.Play("WindSlashLarge");
		}

		//std::cout << attackCount << std::endl;
		//std::cout << comboQueue.front() - prevComboTime << std::endl;
		prevComboTime = comboQueue.front();
		comboQueue.pop();
	}


	// 이걸로 실행
	isCol = Collider->ObbCol(monster->rect);

	// 공격이 닿은 타이밍
	if (player->IsAttack() && isCol && !isAttack)	// 한번이 아닌 실시간으로 실행됨
	{
		// 레이캐스트의 닿은 포지션 구해주기
		raycaster.GetEndPos();
		player->GetMonster()->OnAttacked(1);
		isAttack = true;
	}

	// 중복 공격 방지용
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		isAttack = false;
		isSpawn = false;
		Collider->SetActive(false);
	}
}

void ElementalSpell::RangeUpdate(float dt)
{

	// 공격 스윙
	if (player->IsAttack() && !isSpawn)
	{
		isSpawn = true;
		Collider->SetActive(true);
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider->GetObbCol().setRotation(angle);
		anim.Play("FireBall");
		dir = player->GetLook();

	}


	// 이걸로 실행
	isCol = Collider->ObbCol(monster->rect);

	// 공격이 닿은 타이밍
	if (player->IsAttack() && isCol && !isAttack)	// 한번이 아닌 실시간으로 실행됨
	{
		// 레이캐스트의 닿은 포지션 구해주기
		raycaster.GetEndPos();
		player->GetMonster()->OnAttacked(1);
		isAttack = true;

	}

	// 중복 공격 방지용
	if (isAttack)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		isAttack = false;
		isSpawn = false;
		Collider->SetActive(false);
	}
	


	sf::Vector2f movePos = GetPosition();
	movePos.x += speed * dt;
	movePos.y = GetPosition().y + amplitude * std::sin(2 * M_PI * frequency * (movePos.x / speed));


	SetPosition(movePos);


	Collider->SetPosition(GetPosition());

	//sf::Vector2f movePos = GetPosition();
	//movePos += dir * speed * dt;
	//SetPosition(movePos);
	//Collider->SetPosition(GetPosition());
}

sf::Vector2f ElementalSpell::CalAxisSin(float time, float speed, float frequency, float amplitude, const sf::Vector2f& axis, float angleInDegrees)
{
	float x = time * speed;
	float y = amplitude * std::sin(2 * M_PI * frequency * (x / speed));

	// Calculate rotated position
	float angleInRadians = angleInDegrees * (M_PI / 180.0f);
	float rotatedX = axis.x + x * std::cos(angleInRadians) - y * std::sin(angleInRadians);
	float rotatedY = axis.y + x * std::sin(angleInRadians) + y * std::cos(angleInRadians);

	return sf::Vector2f(rotatedX, rotatedY);
}

void ElementalSpell::CalculatorCurrentTile()
{
	int rowIndex = position.x < 0 ? 0 : position.x / _TileSize;
	int columnIndex = position.y < 0 ? 0 : position.y / _TileSize;

	currentTile = (*wouldTiles)[rowIndex][columnIndex];
}

std::vector<Tile*> ElementalSpell::CalculatorRangeTiles(int row, int col)
{
	//32x16
	int searchRowRange = row;
	int searchColRange = col;

	sf::Vector2i index = currentTile->GetIndex();
	std::vector<Tile*> tiles;

	int topRowIndex = index.x - searchRowRange < 0 ? 0 : index.x > wouldTiles->size() * _TileSize ? wouldTiles->size() * _TileSize : index.x;
	int leftColumnIndex = index.y - searchColRange < 0 ? 0 : index.y > wouldTiles[0].size() * _TileSize ? wouldTiles[0].size() * _TileSize : index.y;
	for (int i = topRowIndex; i < index.x + searchRowRange; i++)
	{
		for (int j = leftColumnIndex; j < index.y + searchColRange; j++)
		{
			tiles.push_back((*this->wouldTiles)[i][j]);
		}
	}

	return tiles;
}
