#pragma once
#include "Singleton.h"
#include "Skill.h"

class SkillMgr : public Singleton<SkillMgr>
{
	friend Singleton<SkillMgr>;

protected:
	SkillMgr() = default;
	virtual ~SkillMgr() override = default;

public:
	// ��ų�� �̺�Ʈ �� ����
	// �װ��� ���� ��ų ���Ƴ��� �뵵
};

#define SKILL_MGR (ResourceMgr::Instance())