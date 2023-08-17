#include "stdafx.h"
#include "SkillMgr.h"

void SkillMgr::InputSkill(Skill* newSkill)
{
	skillList.emplace_back(newSkill);
}
