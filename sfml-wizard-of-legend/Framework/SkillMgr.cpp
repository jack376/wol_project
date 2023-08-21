#include "stdafx.h"
#include "SkillMgr.h"
#include "ElementalSpell.h"
#include "Monster.h"

void SkillMgr::Init()
{
	for (auto skill : skillList)
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
	Skill* useSkill = skillList[sEvent];
	useSkill->UseSkill();
}

void SkillMgr::UseEditorSkill(SkillEvents sEvent)
{
	Skill* useSkill = skillList[sEvent];
	useSkill->UseEditorSkill();

}


void SkillMgr::UseQSkill()
{
	Skill* qSkill = skillList[SkillEvents::Q];
	qSkill->UseSkill();
}

void SkillMgr::UseLeftSkill()
{
	Skill* leftSkill = skillList[SkillEvents::Left];
	leftSkill->UseSkill();

}

void SkillMgr::UseRightSkill()
{
	Skill* rightSkill = skillList[SkillEvents::Right];
	rightSkill->UseSkill();

}

void SkillMgr::UseSpaceSkill()
{
	Skill* spaceSkill = skillList[SkillEvents::Space];
	spaceSkill->UseSkill();

}

void SkillMgr::SwapSkill(SkillEvents sEvent, Skill* equipSkill)
{
	Skill* changeTemp = skillList[sEvent];
	changeTemp->SetSkillEvent(equipSkill->GetSkillEvent());
	equipSkill->SetSkillEvent(sEvent);
}

void SkillMgr::InputSkill(Skill* newSkill)
{
	auto it = skillList.find(newSkill->GetSkillEvent());
	if(it == skillList.end())
		skillList[newSkill->GetSkillEvent()] = newSkill;
	else
	{
		std::cout << "is already" << std::endl;
	}
}
