#include "stdafx.h"
#include "ElementalSpell.h"
#include "SceneGame.h"
#include "Player.h"
#include "Monster.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "Framework.h"

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

	angle = player->GetPlayerLookAngle() + 90;
	// ���ڿ��������� ���� ���ľ���
	// ��ġ �������� �Ǵ� ���� �ִµ� ������ ����
	// 
	// �ϵ� �ڵ���
	// isCol�� �ݶ��̴����� �ε����� true
	// player->IsAttack() �� ��¥ ���� Ÿ�̹� 


	// ���� �ֵθ����� ���� �ʴ� Ÿ�̹�
	// �÷��̾� ������ & ������ �ȵǾ� ������
	// �ִϸ��̼� ��� & Ÿ����ġ ����

	// ���� ����
	if (player->IsAttack() && !isSpawn)
	{
		isSpawn = true;
		collider->SetActive(true);
		collider->SetPosition(player->GetAttackPos());
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		comboQueue.push(FRAMEWORK.GetGamePlayTime());
	}

	// �޺� �ִ� ����
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

		// �ε����� �̿��ؼ� ���� �����̳� �� ������ �ش� �ִϸ��̼� �����ϴ� ��� ����
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

		std::cout << attackCount << std::endl;
		std::cout << comboQueue.front() - prevComboTime << std::endl;
		prevComboTime = comboQueue.front();
		comboQueue.pop();
	}



	// ���� �������� �浹
	isCol = collider->rect.getGlobalBounds().contains(player->GetMonster()->GetPosition());

	// ���� �������� �浹
	//isCol = collider->rect.getGlobalBounds().intersects(player->GetMonster()->sprite.getGlobalBounds());

	// ������ ���� Ÿ�̹�
	if (player->IsAttack() && isCol && !isAttack)	// �ѹ��� �ƴ� �ǽð����� �����
	{
		player->GetMonster()->OnAttacked(1);
		isAttack = true;
	}

	// �ߺ� ���� ������
	if (isSpawn)
	{
		attackTimer += dt;
	}
	
	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		isAttack = false;
		isSpawn = false;
		collider->SetActive(false);
	}
	SetOrigin(Origins::MC);

	anim.Update(dt);
}

void ElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	if (isSpawn)
	{
		collider->SetActive(true);
	}
	else
		collider->SetActive(false);
}
