#include "stdafx.h"
#include "Skill.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ElementalSpell.h"

Skill::Skill(const std::string& textureId, const std::string& n)
	: SpriteGo(textureId, n)
{
}

Skill::~Skill()
{
}

void Skill::Init()
{
	//skillIcon = (SpriteGo*)SCENE_MGR.GetCurrScene()->AddGo(new SpriteGo(skillInfo.skillIconId));

	//if (INPUT_MGR.GetKeyDown(currentKey))
	//{
	//	elementSpell = (ElementalSpell*)SCENE_MGR.GetCurrScene()->AddGo(new ElementalSpell());

	//}
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
	elementSpell = (ElementalSpell*)SCENE_MGR.GetCurrScene()->AddGo(new ElementalSpell());
	elementSpell->sortLayer = 21;
	elementSpell->SetElementType(currentElementType);
	elementSpell->SetSkillType(currentSkillType);
	elementSpell->SetRangeType(currentRangeType);

	// 실제 사용하는 부분에서 적용

	//elementSpell = spellPool.Get();
	//elementSpell->SetTiles(wouldTiles);
	//elementSpell->SetMonsterList(monsters);
	//elementSpell->SetPlayer(this);
}
