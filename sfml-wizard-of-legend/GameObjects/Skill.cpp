#include "stdafx.h"
#include "Skill.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ElementalSpell.h"
#include "SkillEditorElementalSpell.h"
#include "Monster.h"

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
	ElementalSpell* elementalSpell = pool.Get();

	elementalSpell = (ElementalSpell*)SCENE_MGR.GetCurrScene()->AddGo(new ElementalSpell());
	elementalSpell->SetElementType(currentElementType);
	elementalSpell->SetSkillType(currentSkillType);
	elementalSpell->SetRangeType(currentRangeType);
	elementalSpell->SetPlayer(player);
 	elementalSpell->sortLayer = 21;
	elementalSpell->Init();
	elementalSpell->Reset();

	elementalSpell->SetMonsterList(monsters);
	elementalSpell->SetTiles(worldTiles);
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
