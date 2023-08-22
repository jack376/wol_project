#include "stdafx.h"
#include "SkillMgr.h"
#include "ElementalSpell.h"
#include "Monster.h"

void SkillMgr::Init()
{
	for (auto skill : equipSkillList)
	{
		skill.second->SetTiles(worldTiles);
		skill.second->SetMonsterList(monsters);
		skill.second->SetPlayer(player);
		skill.second->SetEditorPlayer(editorPlayer);
		skill.second->Init();
	}
}

void SkillMgr::UseSkill(SkillEvents sEvent)
{
	Skill* useSkill = equipSkillList[sEvent];
	useSkill->UseSkill();
}

void SkillMgr::UseEditorSkill(SkillEvents sEvent)
{
	Skill* useSkill = equipSkillList[sEvent];
	useSkill->UseEditorSkill();

}


void SkillMgr::UseQSkill()
{
	Skill* qSkill = equipSkillList[SkillEvents::Q];
	qSkill->UseSkill();
}

void SkillMgr::UseLeftSkill()
{
	Skill* leftSkill = equipSkillList[SkillEvents::Left];
	leftSkill->UseSkill();

}

void SkillMgr::UseRightSkill()
{
	Skill* rightSkill = equipSkillList[SkillEvents::Right];
	rightSkill->UseSkill();

}

void SkillMgr::UseSpaceSkill()
{
	Skill* spaceSkill = equipSkillList[SkillEvents::Space];
	spaceSkill->UseSkill();

}

void SkillMgr::SwapSkill(SkillEvents sEvent, Skill* equipSkill)
{
	Skill* changeTemp = equipSkillList[sEvent];
	changeTemp->SetSkillEvent(equipSkill->GetSkillEvent());
	equipSkill->SetSkillEvent(sEvent);
}

void SkillMgr::AddSkill(Skill* newSkill)
{
	auto it = existSkillList.find(newSkill->GetSkillId());
	if (it == existSkillList.end())
		existSkillList[newSkill->GetSkillId()] = newSkill;
	else
		std::cout << "is already" << std::endl;
}

void SkillMgr::BuySkill(Skill* newSkill)
{
	auto it = buyedSkillList.find(newSkill->GetSkillId());
	if (it == buyedSkillList.end())
		buyedSkillList[newSkill->GetSkillId()] = newSkill;
	else
		std::cout << "is already" << std::endl;
}

void SkillMgr::InputSkill(Skill* newSkill)
{
	auto it = equipSkillList.find(newSkill->GetSkillEvent());
	if(it == equipSkillList.end())
		equipSkillList[newSkill->GetSkillEvent()] = newSkill;
	else
		std::cout << "is already" << std::endl;
}
