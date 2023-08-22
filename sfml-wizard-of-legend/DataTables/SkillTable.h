#pragma once
#include "DataTable.h"
#include "Skill.h"

class SkillTable : public DataTable
{
protected:
	std::unordered_map<int, SkillInfo> table;

public:
	SkillTable() : DataTable(DataTable::Ids::Monster), table() {}
	virtual ~SkillTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const SkillInfo& Get(int id) const;
};

