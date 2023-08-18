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

	// Ű �Է�

	// ��ų �����Ϳ��� ���̺�� InputSkill�� �̿��ؼ� ����
	std::unordered_map<SkillEvents, Skill*> skillList;

public:
	// ��ų�� �̺�Ʈ �� ����
	// �װ��� ���� ��ų ���Ƴ��� �뵵
	
	//void SkillLoad();
	void UseSkill(SkillEvents sEvent);
	void UseQSkill();
	void UseLeftSkill();
	void UseRightSkill();
	void UseSpaceSkill();
	void SwapSkill(SkillEvents sEvent, Skill* equipSkill);
	void InputSkill(Skill* newSkill);

	// ����Ʈ�� �Ѱܹ޾� ������߿��� Ȯ���ϱ� ���� �Լ�
	std::unordered_map<SkillEvents, Skill*>& ForTestDebugSize() { return skillList; }
};

#define SKILL_MGR (SkillMgr::Instance())