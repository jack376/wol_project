#include "stdafx.h"
#include "rapidcsv.h"
#include "MonsterTable.h"
#include "Monster.h"

const MonsterStat& MonsterTable::Get(int id) const
{
	auto find = table.find(id);
	if (find == table.end())
	{
		//std::cout << "ERR: Undefined ID" << std::endl;
	}
	return find->second;
}

bool MonsterTable::Load()
{
	rapidcsv::Document doc("tables/MonsterInfo.csv");
	std::vector<int> id = doc.GetColumn<int>(0);
	std::vector<MonsterStat> stats;

	std::vector<std::string> names = doc.GetColumn<std::string>(1);
	std::vector<int> types = doc.GetColumn<int>(2);
	std::vector<int> maxHps = doc.GetColumn<int>(3);
	std::vector<float> speeds = doc.GetColumn<float>(4);
	std::vector<int> damages = doc.GetColumn<int>(5);
	std::vector<float> attactRates = doc.GetColumn<float>(6);
	std::vector<float> attactRanges = doc.GetColumn<float>(7);
	std::vector<float> searchRanges = doc.GetColumn<float>(8);
	
	for (int i = 0; i < id.size(); i++)
	{
		stats.push_back({ names[i], (AttactType)types[i], maxHps[i], speeds[i], damages[i], attactRates[i], attactRanges[i], searchRanges[i]});
		table.insert({ id[i], stats[i] });
	}
	return true;
}

void MonsterTable::Release()
{
	table.clear();
}