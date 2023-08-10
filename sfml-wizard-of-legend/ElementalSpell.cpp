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

	// ���Ϸ� �޾Ƽ� �����ϴ� ��� �����ϱ�
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashLarge.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashMed.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashSmall.csv"));

	sprite.setScale(2, 2);
	anim.SetTarget(&sprite);
}

void ElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

	// �ٿ� ������ ������ �������� ��� ���� �÷��̾� ���ݰ� ����ȭ�� �ȉ�
	// �ݶ��̴� ��ġ�� ���� �浹 ��ġ�� �̻���
	// Ÿ�̸ӷ� �ߺ� ������ ����������, �÷��̾� ���ݰ� ����ȭ�� �ȵǴ»���
	//isCol = collider->rect.getGlobalBounds().contains(player->GetMonster()->GetPosition());
	isCol = collider->rect.getGlobalBounds().intersects(player->GetMonster()->sprite.getGlobalBounds());

	// ���ڿ��������� ���� ���ľ���
	// �ϵ� �ڵ���
	if (isCol && !isAttack)
	{
		player->GetMonster()->OnAttacked(1);
		isAttack = true;
	}

	// �ߺ� ���� ������
	if (isAttack)
	{
		attackTimer += dt;
	}
	
	if (attackTimer > attackDuration || !isAttack)
	{
		attackTimer = 0.f;
		isAttack = false;
	}

	if (player->IsAttack())	// �ѹ��� �ƴ� �ǽð����� �����
	{
		isAttack = true;
	}
	else
	{
		isSpawn = false;
		collider->SetActive(false);
	}
	// �� �����Ӹ� �ݶ��̴� ����ȭ
	// ���� �����ͼ� �����̼� �ϱ�
	// ���� ���� �����ؼ� �ؾ���
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
