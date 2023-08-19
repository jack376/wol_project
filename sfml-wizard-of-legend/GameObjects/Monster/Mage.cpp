#include "stdafx.h"
#include "Mage.h"
#include "ResourceMgr.h"
#include "DataTableMgr.h"
#include "MonsterTable.h"
#include "Player.h"

Mage::Mage(MonsterId id, const std::string& textureId, const std::string& n)
	: Monster(id, textureId, n)
{
}

Mage::~Mage()
{
}

void Mage::Init()
{
	stat = DATATABLE_MGR.Get<MonsterTable>(DataTable::Ids::Monster)->Get((int)monsterId);

	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Run.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Death.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Idle.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/" + stat.name + "_Hurt.csv"));

	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Mage_AttackAim.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Mage_AttackShoot.csv"));
	for (auto& fireball : fireballs)
	{
		fireball.AddClip("animations/Mage_FireballCreate.csv");
		fireball.AddClip("animations/Mage_FireballActive.csv");
		fireball.Init();
		fireball.SetTiles(worldTiles);
	}
}

void Mage::Release()
{
	Monster::Release();

}

void Mage::Reset()
{
	Monster::Reset();

	for (auto& fireball : fireballs)
	{
		fireball.Reset();
		fireball.SetOrigin(Origins::MC);
		fireball.SetActive(false);
		fireball.SetPlayer(player);
		fireball.Play("MageFireballCreate");
	}
}

void Mage::Update(float dt)
{
	Monster::Update(dt);
	for (auto& fireball : fireballs)
	{
		if (fireball.GetActive())
			fireball.Update(dt);
	}
}

void Mage::Draw(sf::RenderWindow& window)
{
	Monster::Draw(window);
	for (auto& fireball : fireballs)
	{
		if (fireball.GetActive())
			fireball.Draw(window);
	}
}

void Mage::HandleAttackState(float dt)
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

void Mage::Attack(float dt)
{
	if (attackTimer >= stat.attackRate)
	{
		currentAttackState = AttackState::Aim;
		attackTimer = 0.f;
	}

	HandleAttackState(dt);

	if (hp <= 0)
	{
		SetState(MonsterState::Dead);
		return;
	}
}

void Mage::Aim(float dt)
{
	if (animation.GetCurrentClipId() != "MageAttackAim")
	{
		animation.Play("MageAttackAim");
		SetOrigin(origin);
		SetRectBox();
		
		sf::Vector2f playerPos = player->GetPosition();
		attackDir = SetLook(playerPos);

		std::vector<sf::Vector2f> fireballPoss = CalculateProjectilePositions(playerPos, position, 150, 5, 120);

		for (int i = 0; i < 5; i++)
		{
			fireballs[i].Reset();
			fireballs[i].SetActive(true);
			fireballs[i].SetPosition(fireballPoss[i]);
			fireballs[i].SetRotation(Utils::Angle(fireballPoss[i] - position) - 90);
			fireballs[i].Play("MageFireballCreate");
			fireballs[i].PlayQueue("MageFireballActive");
		}
	}

	aimTimer += dt;

	if (aimTimer > aimRate)
	{
		currentAttackState = AttackState::Shoot;
		aimTimer = 0;
	}
}

void Mage::Shoot(float dt)
{
	if (animation.GetCurrentClipId() != "MageAttackShoot")
	{
		animation.Play("MageAttackShoot");
		for (auto& fireball : fireballs)
		{	
			fireball.Fire(Utils::Normalize(fireball.GetPosition() - position), 500, 20);
		}
	}

	stiffenTimer += dt;

	if (stiffenTimer > stiffenRate)
	{
		SetState(MonsterState::Idle);
		currentAttackState = AttackState::Cool;
		stiffenTimer = 0;
	}	
}

void Mage::KnockBack(float dt)
{
	Monster::KnockBack(dt);
	if (currentAttackState == AttackState::Aim)
	{
		for (auto& fireball : fireballs)
		{
			fireball.Reset();
			fireball.SetActive(false);
		}
	}
}

std::vector<sf::Vector2f> Mage::CalculateProjectilePositions(const sf::Vector2f& playerPosition, const sf::Vector2f& monsterPosition, float radius, int count, float angleRange) {
	std::vector<sf::Vector2f> positions;

	sf::Vector2f direction = playerPosition - monsterPosition;
	direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// 중심 투사체 위치 계산 (플레이어 방향)
	float centerAngle = Utils::Angle(direction);
	// 투사체 위치 계산
	float angleIncrement = angleRange / count;
	float startAngle = centerAngle - angleRange / 2.0f;

	for (int i = 0; i < count; ++i) {
		float adjustedAngle = startAngle + i * angleIncrement;
		sf::Vector2f offset = radius * Utils::Direction(adjustedAngle);
		sf::Vector2f projectilePosition = monsterPosition + offset;
		positions.emplace_back(projectilePosition);
	}
	return positions;
}
