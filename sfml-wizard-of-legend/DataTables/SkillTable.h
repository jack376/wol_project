#pragma once
#include "DataTable.h"
#include "Skill.h"

class SkillTable : public DataTable
{
protected:
	std::vector<std::unordered_map<int, SkillInfo>> tables;

public:
	SkillTable() : DataTable(DataTable::Ids::Skill), tables((int)SkillDatas::Buyed) {}
	virtual ~SkillTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const SkillInfo& Get(int id, SkillDatas dataType) const;
	const std::vector<std::unordered_map<int, SkillInfo>>& GetTable() { return tables; }
};

