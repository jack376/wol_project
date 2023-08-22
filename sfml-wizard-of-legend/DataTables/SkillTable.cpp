#include "stdafx.h"
#include "rapidcsv.h"
#include "SkillTable.h"

const SkillInfo& SkillTable::Get(int id) const
{
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined ID" << std::endl;
	}
	return find->second;
}

bool SkillTable::Load()
{
	rapidcsv::Document doc("SkillTables/SkillInfo.csv");
	std::vector<int> id = doc.GetColumn<int>(0);

	std::vector<SkillInfo> skillInfo;
	std::vector<SpellInfo> spellInfo;

	std::vector<std::string> skillNames = doc.GetColumn<std::string>(1);
	std::vector<int> elementTypes = doc.GetColumn<int>(2);
	std::vector<int> skillTypes = doc.GetColumn<int>(3);
	std::vector<int> rangeTypes = doc.GetColumn<int>(4);
	std::vector<int> eventTypes = doc.GetColumn<int>(5);
	std::vector<int> playerActions = doc.GetColumn<int>(6);
	
	std::vector<int> damages = doc.GetColumn<int>(7);
	std::vector<int> comboDamages = doc.GetColumn<int>(8);
	std::vector<int> comboMaxCounts = doc.GetColumn<int>(9);
	std::vector<float> comboDurations = doc.GetColumn<float>(10);
	std::vector<int> maxSkillChages = doc.GetColumn<int>(11);
	std::vector<int> shotCounts = doc.GetColumn<int>(12);
	
	std::vector<float> speeds = doc.GetColumn<float>(13);
	std::vector<float> ranges = doc.GetColumn<float>(14);
	std::vector<float> explosionRanges = doc.GetColumn<float>(15);
	std::vector<float> amplitudes = doc.GetColumn<float>(16);
	std::vector<float> frequencys = doc.GetColumn<float>(17);
	std::vector<float> delayDurations = doc.GetColumn<float>(18);
	std::vector<float> damageDelays = doc.GetColumn<float>(19);
	std::vector<float> coolTimes = doc.GetColumn<float>(20);
	std::vector<float> rotateSpeeds = doc.GetColumn<float>(21);
	
	std::vector<int> isPenetratings = doc.GetColumn<int>(22);
	std::vector<int> canMoveDuringSkills = doc.GetColumn<int>(23);


	for (int i = 0; i < id.size(); i++)
	{
		//info.push_back({ names[i], (AttactType)types[i], maxHps[i], speeds[i], damages[i], attactRates[i], attactRanges[i], searchRanges[i] });
		//table.insert({ id[i], skillInfo[i] });
	}
	return true;
}

void SkillTable::Release()
{
	table.clear();
}