#include "stdafx.h"
#include "Mage.h"
#include "ResourceMgr.h"
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
	Monster::Init();
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animation/Mage_AttackAim.csv"));
	animation.AddClip(*RESOURCE_MGR.GetAnimationClip("animation/Mage_AttackShoot.csv"));
	for (auto& fireball : fireballs)
	{
		fireball.AddClip("animations/Mage_FireballCreate.csv");
		fireball.AddClip("animations/Mage_FireballActive.csv");
		fireball.Init();
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

		for (auto& fireball : fireballs)
		{
			fireball.SetActive(true);

		}
	}
}

void Mage::Shoot(float dt)
{
}

std::vector<sf::Vector2f> CalculateProjectilePositions(const sf::Vector2f& playerPosition, const sf::Vector2f& monsterPosition, float radius, int count) {
	std::vector<sf::Vector2f> positions;

	// �÷��̾� ������ ���
	sf::Vector2f direction = playerPosition - monsterPosition;
	direction /= std::sqrt(direction.x * direction.x + direction.y * direction.y);

	// �߽� ����ü ��ġ ��� (�÷��̾� ����)
	sf::Vector2f centerPosition = monsterPosition + direction * radius;

	// ������ ����ü ��ġ ���
	float angleIncrement = 2 * 3.14159265358979323846 / (count - 1); // �߽� �����ϹǷ� count-1
	for (int i = 0; i < count - 1; ++i) {
		float angle = i * angleIncrement;
		float x = monsterPosition.x + radius * std::cos(angle);
		float y = monsterPosition.y + radius * std::sin(angle);
		positions.emplace_back(x, y);
	}

	// �߽� ����ü ��ġ�� �� �տ� �߰�
	positions.insert(positions.begin(), centerPosition);

	return positions;
}
