#include "stdafx.h"
#include "Skill.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ElementalSpell.h"
#include "SkillEditorElementalSpell.h"
#include "Monster.h"
#include "Player.h"

Skill::Skill(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

Skill::~Skill()
{
}

void Skill::Init()
{
	pool.OnCreate = [this](ElementalSpell* spell)
	{
		spell->SetMonsterList(monsters);
		spell->SetPool(pool);
	};
	editorPool.OnCreate = [this](SkillEditorElementalSpell* spell)
	{
		spell->SetPool(editorPool);
	};

	spellInfo = skillInfo.spellinfo;
	skillId = skillInfo.skillId;
	skillName = spellInfo.skillName;
	currentElementType = skillInfo.elementType;
	currentSkillType = skillInfo.skillType;
	currentRangeType = skillInfo.rangeType;
	currentEventType = skillInfo.evnetType;
	currentPlayerActionType = skillInfo.playerAction;
}

void Skill::Release()
{
}

void Skill::Reset()
{
	SpriteGo::Reset();
}

void Skill::Update(float dt)
{
	SpriteGo::Update(dt);
}

void Skill::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);

}

void Skill::UseSkill()
{
	shotCount = spellInfo.shotCount;
	coolTime = spellInfo.coolTime;
	spreadAngle = spellInfo.spreadAngle;
	maxSkillCharge = spellInfo.maxSkillCharge;


	// 중심 투사체 위치 계산 (플레이어 방향)
	float centerAngle = player->GetPlayerLookAngle(); //360도 기준

	// 투사체 위치 계산
	float angleIncrement = spreadAngle / shotCount;
	float startAngle = centerAngle - spreadAngle / 2.0f;

	if (shotCount > 1)
	{
		// 각도 설정
		for (int i = 0; i < shotCount; i++)
		{
			float adjustedAngle = startAngle + i * angleIncrement;
			
			ElementalSpell* elementalSpell = pool.Get();
			elementalSpell = (ElementalSpell*)SCENE_MGR.GetCurrScene()->AddGo(new ElementalSpell());
			elementalSpell->SetElementType(currentElementType);
			elementalSpell->SetSkillType(currentSkillType);
			elementalSpell->SetRangeType(currentRangeType);
			elementalSpell->SetSpellInfo(spellInfo);
			elementalSpell->SetPlayer(player);
			elementalSpell->SetDir(Utils::Angle(adjustedAngle));

			if (skillId == 6 || skillId == 7)
			{
				elementalSpell->SetAngle(adjustedAngle + 90);
			}
			else
				elementalSpell->SetAngle(adjustedAngle);


			elementalSpell->sortLayer = 21;
			elementalSpell->Init();
			elementalSpell->Reset();
			elementalSpell->SetMonsterList(monsters);
			elementalSpell->SetTiles(worldTiles);
		}
	}
	else
	{
		ElementalSpell* elementalSpell = pool.Get();
		elementalSpell = (ElementalSpell*)SCENE_MGR.GetCurrScene()->AddGo(new ElementalSpell());
		elementalSpell->SetElementType(currentElementType);
		elementalSpell->SetSkillType(currentSkillType);
		elementalSpell->SetRangeType(currentRangeType);
		elementalSpell->SetSpellInfo(spellInfo);
		elementalSpell->SetPlayer(player);
		elementalSpell->SetDir(player->GetLook());
		elementalSpell->SetAngle(player->GetPlayerLookAngle());

		elementalSpell->sortLayer = 21;
		elementalSpell->Init();
		elementalSpell->Reset();
		elementalSpell->SetMonsterList(monsters);
		elementalSpell->SetTiles(worldTiles);
	}
}

void Skill::UseEditorSkill()
{
	SkillEditorElementalSpell* elementalSpell = editorPool.Get();

	elementalSpell = (SkillEditorElementalSpell*)SCENE_MGR.GetCurrScene()->AddGo(new SkillEditorElementalSpell());
	elementalSpell->SetElementType(currentElementType);
	elementalSpell->SetSkillType(currentSkillType);
	elementalSpell->SetRangeType(currentRangeType);
	elementalSpell->SetPlayer(editorPlayer);
	elementalSpell->sortLayer = 21;
	elementalSpell->Init();
	elementalSpell->Reset();
}
