#pragma once
#include "DataTable.h"
#include "Monster.h"

class MonsterTable :
    public DataTable
{
protected:
	std::unordered_map<int, MonsterStat> table;

public:
	MonsterTable() : DataTable(DataTable::Ids::Monster), table() {}
	virtual ~MonsterTable() override { Release(); }

	virtual bool Load() override;
	virtual void Release() override;

	const MonsterStat& Get(int id) const;
};
