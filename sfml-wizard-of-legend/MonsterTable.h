#pragma once
#include "DataTable.h"
class MonsterTable :
    public DataTable
{
protected:
	std::unordered_map<int, MonsterInfo> table;

public:
	MonsterTable() : DataTable(DataTable::Ids::Monster), table() {}
	virtual ~MonsterTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const MonsterInfo& Get(int id) const;
};
