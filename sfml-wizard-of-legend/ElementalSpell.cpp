#include "stdafx.h"
#include "ElementalSpell.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "ResourceMgr.h"

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
	collider->rect.setSize({ 100, 100 });
	collider->SetPosition(0, 0);
	collider->SetActive(false);

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
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

	// 꾸욱 누르고 있으면 데미지가 계속 들어간다 플레이어 공격과 동기화가 안됌
	// 콜라이더 위치와 실제 충돌 위치가 이상함
	// 타이머로 중복 공격은 방지하지만, 플레이어 공격과 동기화가 안되는상태
	//isCol = collider->rect.getGlobalBounds().contains(player->GetMonster()->GetPosition());
	isCol = collider->rect.getGlobalBounds().intersects(player->GetMonster()->sprite.getGlobalBounds());

	// 부자연스러움이 있음 고쳐야함
	// 하드 코딩용
	if (isCol && !isAttack)
	{
		player->GetMonster()->OnAttacked(1);
		isAttack = true;
	}

	// 중복 공격 방지용
	if (isAttack)
	{
		attackTimer += dt;
	}
	
	if (attackTimer > attackDuration || !isAttack)
	{
		attackTimer = 0.f;
		isAttack = false;
	}

	if (player->IsAttack())	// 한번이 아닌 실시간으로 실행됨
	{
		isAttack = true;
	}
	else
	{
		isSpawn = false;
		collider->SetActive(false);
	}
	// 한 프레임만 콜라이더 가시화
	// 각도 가져와서 로테이션 하기
	// 원래 각도 생각해서 해야함
	if (!isSpawn && isAttack)
	{
		isSpawn = true;
		collider->SetActive(true);
		collider->SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
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
		attackCount++;
	}

	anim.Update(dt);
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if (isAttack)
	{
		collider->SetActive(true);
	}
	else
		collider->SetActive(false);
}
