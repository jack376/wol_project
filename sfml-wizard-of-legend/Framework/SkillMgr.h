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
	std::list<Skill*> skillList;

public:
	// ��ų�� �̺�Ʈ �� ����
	// �װ��� ���� ��ų ���Ƴ��� �뵵
	
	//void SkillLoad();
	void InputSkill(Skill* newSkill);
};

#define SKILL_MGR (ResourceMgr::Instance())