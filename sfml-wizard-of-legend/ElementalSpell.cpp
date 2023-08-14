#include "stdafx.h"
#include "ElementalSpell.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "Framework.h"
#include "BoxCollider2D.h"

ElementalSpell::ElementalSpell(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

ElementalSpell::~ElementalSpell()
{
}

void ElementalSpell::Init()
{
	collider = (SpriteGo*)scene->AddGo(new SpriteGo());
	collider->SetOrigin(Origins::MC);
	collider->rect.setOutlineThickness(1.f);
	collider->rect.setOutlineColor(sf::Color::Blue);
	collider->rect.setFillColor(sf::Color::Transparent);
	collider->rect.setSize({ 100, 80 });
	collider->SetPosition(0, 0);
	collider->SetActive(false);

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

	sprite.setScale(2, 2);
	anim.SetTarget(&sprite);

	Collider->SetSprite(sprite);
	Collider->SetActive(false);
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

	Collider->SetSprite(sprite);
	Collider->SetColSize();

	angle = player->GetPlayerLookAngle() + 90;
	// 부자연스러움이 있음 고쳐야함
	// 설치 느낌으로 되는 감이 있는데 나쁘진 않음
	// 
	// 하드 코딩용
	// isCol은 콜라이더끼리 부딪혀야 true
	// player->IsAttack() 가 진짜 공격 타이밍 


	// 공격 휘두르지만 맞지 않는 타이밍
	// 플레이어 공격중 & 스폰이 안되어 있을때
	// 애니메이션 재생 & 타격위치 설정

	// 공격 스윙
	if (player->IsAttack() && !isSpawn)
	{
		isSpawn = true;
		Collider->SetActive(true);
		Collider->SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
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



	// 점을 기준으로 충돌	new를 안하고 했을때
	//isCol = obbManager.ObbCol(collider->rect, monster->rect);
	


	//isCol = collider->rect.getGlobalBounds().contains(player->GetMonster()->GetPosition());

	// 면을 기준으로 충돌
	//isCol = collider->rect.getGlobalBounds().intersects(player->GetMonster()->sprite.getGlobalBounds());

	// 공격이 닿은 타이밍
	if (player->IsAttack() && isCol && !isAttack)	// 한번이 아닌 실시간으로 실행됨
	{
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

	anim.Update(dt);
	SetOrigin(Origins::MC);
	Collider->SetOrigin(Origins::MC);
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if (isSpawn)
	{
		Collider->SetActive(true);
	}
	else
		Collider->SetActive(false);
}
