#include "stdafx.h"
#include "Archer.h"
#include "ResourceMgr.h"
#include "Player.h"
#include "Tile.h"
#include "AS.h"

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
	arrow.Init();
	arrow.SetTiles(tilesWorld);
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
	arrow.SetPlayer(player);
}

void Archer::Update(float dt)
{
	Monster::Update(dt);
	if (arrow.GetActive())
		arrow.Update(dt);
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
	if (currentAttackState == AttackState::Aim)
		bulletLine.draw(window);
}


void Archer::HandleAttackState(float dt)
{
	switch (currentAttackState)
	{
	case AttackState::Aim:
		Aim(dt);
		break;
	case AttackState::Shoot:
		Shoot(dt);
		break;
	default:
		std::cout << "HandleAttackState default" << std::endl;
		break;
	}
}

void Archer::Attack(float dt)
{
	attackArmAni.Update(dt);
	pullArmAni.Update(dt);
	bowAni.Update(dt);

	if (attackTimer >= stat.attackRate)
	{
		if (!raycaster.checkCollision(*nongroundTiles, player))
		{
			SetState(MonsterState::Moving);
			return;
		}
		currentAttackState = AttackState::Aim;
		attackTimer = 0.f;
	}

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
	
	HandleAttackState(dt);

	if (hp <= 0)
	{
		SetState(MonsterState::Dead);
		return;
	}
}

void Archer::Aim(float dt)
{
	if (animation.GetCurrentClipId() != "ArcherAttack")
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
	}
	sf::Vector2f playerPos = player->GetPosition();
	SetLook(playerPos);

	bow.setPosition(attackArm.getPosition());
	arrow.SetPosition(bow.getPosition());
	bulletLine.move(position.x, position.y);

	float angle = Utils::Angle(look);
	attackArm.setRotation(angle);
	pullArm.setRotation(angle);
	bow.setRotation(angle);
	arrow.sprite.setRotation(angle + 90);
	bulletLine.Rotation(angle);

	bulletLine.checkCollision(CalculatorRangeTiles(16, 16), player);

	aimTimer += dt;

	if (aimTimer > aimRate)
	{
		currentAttackState = AttackState::Shoot;
		aimTimer = 0;
	}
}

void Archer::Shoot(float dt)
{
	if (bowAni.GetCurrentClipId() != "ArcherBowRelease")
	{
		bowAni.Play("ArcherBowRelease");
		bow.setOrigin(-_BowLocalPos);
		arrow.Fire(bow.getPosition(), look, arrowSpeed, stat.damage);
	}

	if (bowAni.IsAnimEndFrame())
	{
		currentAttackState = AttackState::Cool;
		SetState(MonsterState::Idle);
	}
}

void Archer::Move(float dt)
{
	Monster::Move(dt);

	if (!raycaster.checkCollision(*nongroundTiles, player))
		SetState(MonsterState::Moving);

}
