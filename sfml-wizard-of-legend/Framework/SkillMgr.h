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

	// 키 입력

	// 스킬 에디터에서 세이브시 InputSkill을 이용해서 저장
	std::unordered_map<SkillEvents, Skill*> skillList;

public:
	// 스킬의 이벤트 값 정보
	// 그것을 토대로 스킬 갈아끼는 용도
	
	//void SkillLoad();
	void UseSkill(SkillEvents sEvent);
	void UseQSkill();
	void UseLeftSkill();
	void UseRightSkill();
	void UseSpaceSkill();
	void SwapSkill(SkillEvents sEvent, Skill* equipSkill);
	void InputSkill(Skill* newSkill);

	// 리스트를 넘겨받아 디버그중에서 확인하기 위한 함수
	std::unordered_map<SkillEvents, Skill*>& ForTestDebugSize() { return skillList; }
};

#define SKILL_MGR (SkillMgr::Instance())