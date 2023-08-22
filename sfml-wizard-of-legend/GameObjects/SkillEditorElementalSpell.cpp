#include "stdafx.h"
#include "SkillEditorElementalSpell.h"
#include "SceneGame.h"
#include "SkillEditorPlayer.h"
#include "Monster.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "Framework.h"
#include "Tile.h"
#include "SceneMgr.h"

SkillEditorElementalSpell::SkillEditorElementalSpell(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

SkillEditorElementalSpell::~SkillEditorElementalSpell()
{
}

void SkillEditorElementalSpell::Init()
{

}

void SkillEditorElementalSpell::Release()
{
}

void SkillEditorElementalSpell::Reset()
{
	SpriteGo::Reset();

	// ���Ϸ� �޾Ƽ� �����ϴ� ��� �����ϱ�
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashLarge.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashMed.csv"));
	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/WindSlash/WindSlashSmall.csv"));

	anim.AddClip(*RESOURCE_MGR.GetAnimationClip("animations/Player/Attck_Basic/FireBall/FireBall.csv"));

	sprite.setScale(2, 2);
	anim.SetTarget(&sprite);
}

void SkillEditorElementalSpell::Update(float dt)
{
	SpriteGo::Update(dt);

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
}

void SkillEditorElementalSpell::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
}

void SkillEditorElementalSpell::MeleeUpdate(float dt)
{
	// isCol�� �ݶ��̴����� �ε����� true
	// player->IsAttack() �� ��¥ ���� Ÿ�̹� 

	// ���� ����
	if (!isSpawn)
	{
		isSpawn = true;
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

		//std::cout << attackCount << std::endl;
		//std::cout << comboQueue.front() - prevComboTime << std::endl;
		prevComboTime = comboQueue.front();
		comboQueue.pop();
	}


	// �ߺ� ���� ������
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
	}

	// �����Ÿ� ���̸� isSpawn false��


	// isSpawn �� false�� �Ǹ� �������
	if (!isSpawn)
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool.Return(this);
	}
}

void SkillEditorElementalSpell::RangeUpdate(float dt)
{

	// ���� ����
	if (!isSpawn)
	{
		isSpawn = true;
		SetPosition(player->GetAttackPos());
		sprite.setRotation(angle);
		anim.Play("FireBall");
		dir = player->GetLook();
		curveAngle = angle;
		time = 0.f;
	}


	// �ߺ� ���� ������
	if (isSpawn)
	{
		attackTimer += dt;
	}

	if (attackTimer > attackDuration)
	{
		attackTimer = 0.f;
		//isSpawn = false;
	}
	// �����Ÿ� ���̸� isSpawn false��



// isSpawn �� false�� �Ǹ� �������
	if (!isSpawn)
	{
		SCENE_MGR.GetCurrScene()->RemoveGo(this);
		pool.Return(this);
	}

	// � ���� ǥ��
	switch (currentRangeType)
	{
	case RangeTypes::Straight:
		StraightUpdate(dt);
		break;

	case RangeTypes::Curve:
		CurveUpdate(dt);
		break;
	}


}

void SkillEditorElementalSpell::CurveUpdate(float dt)
{
	time += dt;
	sf::Vector2f movePos = GetPosition() + CalAxisSin(time, curveSpeed, frequency, amplitude, dir, curveAngle);
	SetPosition(movePos);
}

void SkillEditorElementalSpell::StraightUpdate(float dt)
{
	sf::Vector2f movePos = GetPosition();
	movePos += dir * speed * dt;
	SetPosition(movePos);
}

sf::Vector2f SkillEditorElementalSpell::CalAxisSin(float time, float speed, float frequency, float amplitude, const sf::Vector2f& axis, float angleInDegrees)
{
	float x = time * speed;
	float y = amplitude * std::sin(2 * M_PI * frequency * (x / speed));

	// Calculate rotated position
	float angleInRadians = angleInDegrees * (M_PI / 180.0f);
	float rotatedX = axis.x + x * std::cos(angleInRadians) - y * std::sin(angleInRadians);
	float rotatedY = axis.y + x * std::sin(angleInRadians) + y * std::cos(angleInRadians);

	return sf::Vector2f(rotatedX, rotatedY);
}