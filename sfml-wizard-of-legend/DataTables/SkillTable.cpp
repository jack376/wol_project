#include "stdafx.h"
#include "rapidcsv.h"
#include "SkillTable.h"

const SkillInfo& SkillTable::Get(int id, SkillDatas dataType) const
{
	auto& table = tables[(int)dataType];
	auto find = table.find(id);
	if (find == table.end())
	{
		std::cout << "ERR: Undefined ID" << std::endl;
	}
	return find->second;
}

bool SkillTable::Load()
{
	std::vector<std::string> fileNames;
	fileNames.push_back("tables/ExistedSkillInfo.csv");
	//fileNames.push_back("tables/BuyedSkillInfo.csv");
	//fileNames.push_back("tables/EquipedSkillInfo.csv");

	//rapidcsv::Document doc("tables/ExistedSkillInfo.csv");

	for (int i = 0; i < tables.size(); ++i)
	{
		rapidcsv::Document doc(fileNames[i]);
		std::vector<int> id = doc.GetColumn<int>(0);


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

		std::vector<SkillInfo> skillInfos;
		std::vector<SpellInfo> spellInfos;

		for (int j = 0; j < id.size(); j++)
		{
			spellInfos.push_back({
				skillNames[j],
				damages[j],
				comboDamages[j],
				comboMaxCounts[j],
				comboDurations[j],
				maxSkillChages[j],
				shotCounts[j],
				speeds[j],
				ranges[j],
				explosionRanges[j],
				amplitudes[j],
				frequencys[j],
				delayDurations[j],
				damageDelays[j],
				coolTimes[j],
				rotateSpeeds[j],
				(bool)isPenetratings[j],
				(bool)canMoveDuringSkills[j] });

			skillInfos.push_back({ id[j],
				(ElementTypes)elementTypes[j],
				(SkillTypes)skillTypes[j],
				(RangeTypes)rangeTypes[j],
				(SkillEvents)eventTypes[j],
				(PlayerActions)playerActions[j],
				spellInfos[j] });

			tables[i].insert({id[j], skillInfos[j]});
		}
	}
	return true;
}

void SkillTable::Release()
{
	for (auto table : tables)
	{
		table.clear();
	}
}