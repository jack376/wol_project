#pragma once
#include "Singleton.h"
#include "Skill.h"

class Skill;

class SkillMgr : public Singleton<SkillMgr>
{
	friend Singleton<SkillMgr>;

protected:
	SkillMgr() = default;
	virtual ~SkillMgr() override = default;

	std::list<Skill*> skillList;

public:
	// 스킬의 이벤트 값 정보
	// 그것을 토대로 스킬 갈아끼는 용도
	
	//void SkillLoad();
	void InputSkill(Skill* newSkill);
};

#define SKILL_MGR (ResourceMgr::Instance())