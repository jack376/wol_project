#include "stdafx.h"
#include "Archer.h"
#include "ResourceMgr.h"
#include "Player.h"


#define _AttackArmLocalPos sf::Vector2f(44, 44)
#define _PullArmLocalPos sf::Vector2f(0, 42)
#define _BowLocalPos sf::Vector2f(-6, -14)


Archer::Archer(MonsterId id, const std::string& textureId, const std::string& n)
	: Monster(id, textureId, n), bulletLine()
{
}

Archer::~Archer()
{
}

void Archer::Init()
{
	Monster::Init();
	bowAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Archer_BowPull.csv"));
	bowAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Archer_BowRelease.csv"));
	attackArmAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Archer_AttackArm.csv"));
	pullArmAni.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Archer_AttackPullArm.csv"));
	arrow.textureId = "graphics/Texture2D/Arrow2.png";
}

void Archer::Release()
{
	Monster::Release();
}

void Archer::Reset()
{
	Monster::Reset();

	bowAni.SetTarget(&bow);
	attackArmAni.SetTarget(&attackArm);
	pullArmAni.SetTarget(&pullArm);
	bow.setScale({ 4.f, 4.f });
	attackArm.setScale({ 4.f, 4.f });
	pullArm.setScale({ 4.f, 4.f });

    bow.setOrigin(-_BowLocalPos);
	Utils::SetOrigin(attackArm, Origins::ML);
	Utils::SetOrigin(pullArm, Origins::ML);

	arrow.Reset();
	arrow.SetOrigin(Origins::BC);
	arrow.SetActive(false);
}

void Archer::Update(float dt)
{
	Monster::Update(dt);
	if (currentState == MonsterState::Attacking)
	{
		attackArmAni.Update(dt);
		pullArmAni.Update(dt);
		bowAni.Update(dt);

		if (isAme)
		{
			SetLook(player->GetPosition());
			float angle = Utils::Angle(look);
			attackArm.setRotation(angle);
			pullArm.setRotation(angle);
			bow.setRotation(angle);
			arrow.sprite.setRotation(angle + 90);
			bulletLine.Rotation(angle);

			sf::Vector2f pos = { sprite.getGlobalBounds().left, sprite.getGlobalBounds().top };
			if (look.x < 0)
			{
				pos.x += sprite.getGlobalBounds().width;
				attackArm.setPosition(pos.x + -_AttackArmLocalPos.x, pos.y + _AttackArmLocalPos.y);
				pullArm.setPosition(pos.x + -_PullArmLocalPos.x, pos.y + _PullArmLocalPos.y);
			}
			else
			{
				attackArm.setPosition(pos + _AttackArmLocalPos);
				pullArm.setPosition(pos + _PullArmLocalPos);
			}
			bow.setPosition(attackArm.getPosition());
			arrow.SetPosition(bow.getPosition());
			bulletLine.move(position.x, position.y);
			
			bulletLine.checkCollision(CalculatorRangeTiles(16, 8), player);
		}

		if (arrow.GetActive())
			arrow.Update(dt);
	}
}

void Archer::Draw(sf::RenderWindow& window)
{
	if (currentState == MonsterState::Attacking)
		window.draw(attackArm);
	Monster::Draw(window);
	if (currentState == MonsterState::Attacking)
	{
		window.draw(pullArm);
		window.draw(bow);
	}
	if (arrow.GetActive())
		arrow.Draw(window);
	if (isAme)
		bulletLine.draw(window);
}

void Archer::Attack(float dt)
{
    SetState(MonsterState::Attacking);
    if (attackTimer >= stat.attackRate)
    {
        animation.Play("ArcherAttack");
        attackArmAni.Play("ArcherAttackArm");
        pullArmAni.Play("ArcherAttackPullArm");
        bowAni.Play("ArcherBowPull");

        SetOrigin(origin);
        SetRectBox();

		Utils::SetOrigin(attackArm, Origins::ML);
		Utils::SetOrigin(pullArm, Origins::ML);
		bow.setOrigin(-_BowLocalPos);
		arrow.SetActive(true);

        attackTimer = 0.f;
        isAttacked = false;
        isAttacking = true;
		isAme = true;
    }
	if (isAme)
		ameTimer += dt;
	if (ameTimer >= ameRate)
	{
		bowAni.Play("ArcherBowRelease");
		bow.setOrigin(-_BowLocalPos);
		arrow.Fire(bow.getPosition(), look, arrowSpeed);
		ameTimer = 0;
		isAme = false;
	}
}
