#include "stdafx.h"
#include "ElementalSpell.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "Framework.h"
#include "Tile.h"
#include "SceneMgr.h"
#include "SpriteEffect.h"

ElementalSpell::ElementalSpell(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

ElementalSpell::~ElementalSpell()
{

}

void ElementalSpell::Init()
{
	Collider.Init();
	monsterHitEffectPool.OnCreate = [this](SpriteEffect* effect) {
		effect->sprite.setScale(3, 3);
		effect->SetAnimId("AttackEffect");
		effect->SetDuration(0.5f);
		effect->sortLayer = 20;
	};
	monsterHitEffectPool.Init();
}

void ElementalSpell::Release()
{
	for (auto obj : monsterHitEffectPool.GetUseList())
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(obj);
	}
	monsterHitEffectPool.Clear();
}

void ElementalSpell::Reset()
{
	SpriteGo::Reset();
	//for (auto obj : monsterHitEffectPool.GetUseList())
	//{
	//	scene->RemoveGo(obj);
	//}
	monsterHitEffectPool.Clear();

	// 파일로 받아서 실행하는 방법 생각하기
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlash.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/ExplodingFireball/ExplodingFireball.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/FrostFan/FrostFan.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/IceDagger/IceDagger.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/GustVolley/GustVolley.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/VoltDisc/VoltDisc.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/DragonArc/DragonArc.csv"));
	
	amplitude = currentInfo.amplitude;
	damage = currentInfo.damage;
	attackDuration = currentInfo.delayDuration;
	currentInfo.explosionRange;
	frequency = currentInfo.frequency;
	range = currentInfo.range;
	currentAnimId = currentInfo.skillName;
	speed = currentInfo.speed;
	curveSpeed = currentInfo.speed;

	if (currentAnimId.compare("FrostFan") == 0)
	{
		sprite.setScale(2, 3);
	}
	else if (currentAnimId.compare("GustVolley") == 0)
	{
		sprite.setScale(4, 4);
	}
	else if (currentAnimId.compare("VoltDisc") == 0)
	{
		sprite.setScale(4, 4);
	}
	else if (currentAnimId.compare("DragonArc") == 0)
	{
		sprite.setScale(4, 4);
	}
	else if (currentAnimId.compare("IceDagger") == 0)
	{
		sprite.setScale(4, 4);
	}
	else
		sprite.setScale(2, 2);

	anim.SetTarget(&sprite);
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);
	CalculatorCurrentTile();

	Collider.SetSprite(sprite);
	Collider.SetColSize();


	raycaster.move(position.x, position.y);
	for (auto& monster : *monsters)
	{
		if(monster->GetActive())
			raycaster.checkCollision(monster);
	}

	switch (currentSkillType)
	{
	case SkillTypes::Melee:
		angle = player->GetPlayerLookAngle() + 90;
		MeleeUpdate(dt);
		break;

	case SkillTypes::Range:
		//angle = player->GetPlayerLookAngle() + 90;
		RangeUpdate(dt);
		break;
	}



	anim.Update(dt);
	SetOrigin(Origins::MC);
	Collider.SetOrigin(Origins::MC);
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	//if(Collider.GetActive())
	//	Collider.Draw(window);
	//raycaster.draw(window);
	/*
	if (isSpawn)
	{
		Collider.Draw(window);
		raycaster.draw(window);
	}
	*/
}

void ElementalSpell::MeleeUpdate(float dt)
{
	// 공격 스윙
	if (!isSpawn)
	{
		isSpawn = true;
		Collider.SetActive(true);
		Collider.SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider.GetObbCol().setRotation(angle);
		comboQueue.push(FRAMEWORK.GetGamePlayTime());
	}

	// 콤보 넣는 구간
	if (!comboQueue.empty())
	{
		if (comboQueue.front() - prevComboTime > comboDuration)
		{
			attackCount = 0;
		}

		if (comboQueue.front() - prevComboTime < comboDuration)
		{
			attackCount++;
		}
		anim.Play(currentAnimId);

		// 인덱스를 이용해서 벡터 컨테이너 값 참조후 해당 애니메이션 실행하는 방법 생각
		//if (attackCount % 3 == 0)
		//{
		//	anim.Play("WindSlashSmall");
		//}
		//if (attackCount % 3 == 1)
		//{
		//	anim.Play("WindSlashMed");
		//}
		//if (attackCount % 3 == 2)
		//{
		//	anim.Play("WindSlashLarge");
		//}

		prevComboTime = comboQueue.front();
		comboQueue.pop();
	}


	// 이걸로 실행
	for (auto& monster : *monsters)
	{
		isCol = Collider.ObbCol(monster->rect) && monster->GetActive();

		auto it = std::find(colMonsters.begin(), colMonsters.end(), monster);

		// 공격이 닿은 타이밍
		if (isCol && it == colMonsters.end())	// 한번이 아닌 실시간으로 실행됨
		{
			// 레이캐스트의 닿은 포지션 구해주기
			attackTimer = 0.f;
			SpriteEffect* monsterHitEffect = monsterHitEffectPool.Get();
			monsterHitEffect->SetPosition(raycaster.GetEndPos());
			float randAngle = Utils::RandomRange(0.f, 360.f);
			monsterHitEffect->sprite.setRotation(randAngle);
			SCENE_MGR.GetCurrScene()->AddGo(monsterHitEffect);
			monster->OnAttacked(damage);
			colMonsters.push_back(monster);
		}
	}

	// 중복 공격 방지용
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;

		//isSpawn = false;
		Collider.SetActive(false);
	}

	// 사정거리 밖이면 isSpawn false로

	range -= speed * dt;
	if (range < 0)
	{
		isSpawn = false;
	}

	// isSpawn 이 false가 되면 사라진다
	if (!isSpawn)
	{
		colMonsters.clear();
		//pool->Return(this);
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
	}
}

void ElementalSpell::RangeUpdate(float dt)
{
	// 공격 스윙
	if (!isSpawn)
	{
		isSpawn = true;
		Collider.SetActive(true);
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		raycaster.Rotation(player->GetPlayerLookAngle());
		Collider.GetObbCol().setRotation(angle);
		anim.Play(currentAnimId);
		//dir = player->GetLook();
		curveAngle = angle;
		time = 0.f;
	}


	// 이걸로 실행
	for (auto& monster : *monsters)
	{
		isCol = Collider.ObbCol(monster->rect);
		auto it = std::find(colMonsters.begin(), colMonsters.end(), monster);

		// 공격이 닿은 타이밍
		if (isCol && it == colMonsters.end())	// 한번이 아닌 실시간으로 실행됨
		{
			// 레이캐스트의 닿은 포지션 구해주기
			SpriteEffect* monsterHitEffect = monsterHitEffectPool.Get();
			monsterHitEffect->SetPosition(raycaster.GetEndPos());
			float randAngle = Utils::RandomRange(0.f, 360.f);
			monsterHitEffect->sprite.setRotation(randAngle);
			SCENE_MGR.GetCurrScene()->AddGo(monsterHitEffect);
			monster->OnAttacked(damage);
			monster->SetIsHit(true);
			colMonsters.push_back(monster);
		}
	}

	if (currentTile->GetType() == TileType::Wall)
	{
		SpriteEffect* wallHitEffect = monsterHitEffectPool.Get();
		wallHitEffect->SetPosition(GetPosition());
		float randAngle = Utils::RandomRange(0.f, 360.f);
		wallHitEffect->sprite.setRotation(randAngle);
		SCENE_MGR.GetCurrScene()->AddGo(wallHitEffect);
		isSpawn = false;
	}



	// 중복 공격 방지용
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		//isSpawn = false;
		Collider.SetActive(false);
	}




	
	// 곡선 직선 표현
	switch (currentRangeType)
	{
	case RangeTypes::Straight:
		StraightUpdate(dt);
		break;

	case RangeTypes::Curve:
		CurveUpdate(dt);
		break;
	}


	// 사정거리 밖이면 isSpawn false로
	// isSpawn 이 false가 되면 사라진다

	range -= speed * dt;
	if (range < 0)
	{
		isSpawn = false;
	}

	if (!isSpawn)
	{
		//pool->Return(this);
		colMonsters.clear();
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
	}

}

void ElementalSpell::CurveUpdate(float dt)
{
	time += dt;
	sf::Vector2f sinAngle = CalAxisSin(time, curveSpeed, frequency, amplitude, dir, curveAngle);
	sf::Vector2f movePos = GetPosition() + sinAngle;

	angle = Utils::Angle(sinAngle);

	if(player->GetPlayerLookAngle() >= 90 && player->GetPlayerLookAngle() <= 270)
		SetFlipY(false);
		
	sprite.setRotation(angle);
	SetPosition(movePos);
	Collider.SetPosition(GetPosition());
}

void ElementalSpell::StraightUpdate(float dt)
{
	sf::Vector2f movePos = GetPosition();
	movePos += dir * speed * dt;
	SetPosition(movePos);
	Collider.SetPosition(GetPosition());
}

sf::RectangleShape& ElementalSpell::GetCollider()
{
	return Collider.GetObbCol();
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

	currentTile = (*worldTiles)[rowIndex][columnIndex];
}

std::vector<Tile*> ElementalSpell::CalculatorRangeTiles(int row, int col)
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
