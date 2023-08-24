#include "stdafx.h"
#include "rapidcsv.h"
#include "SkillMgr.h"
#include "ElementalSpell.h"
#include "Monster.h"
#include "DataTableMgr.h"
#include "SkillTable.h"

void SkillMgr::Init()
{
	// 모든 스킬 Load
	LoadExistSkill();
	if(!buyedSkillList.empty())
		LoadBuyedSkill();
	if (!equipedSkillList.empty())
		LoadEquipedSkill();

	for (auto skill : equipedSkillList)
	{
		skill.second->SetTiles(worldTiles);
		skill.second->SetMonsterList(monsters);
		skill.second->SetPlayer(player);
		skill.second->SetEditorPlayer(editorPlayer);
		skill.second->Init();
	}
}

void SkillMgr::UseSkill(SkillEvents sEvent)
{
	Skill* useSkill = equipedSkillList[sEvent];
	useSkill->UseSkill();
}

void SkillMgr::UseEditorSkill(SkillEvents sEvent)
{
	Skill* useSkill = equipedSkillList[sEvent];
	useSkill->UseEditorSkill();

}


void SkillMgr::UseQSkill()
{
	Skill* qSkill = equipedSkillList[SkillEvents::Q];
	qSkill->UseSkill();
}

void SkillMgr::UseLeftSkill()
{
	Skill* leftSkill = equipedSkillList[SkillEvents::Left];
	leftSkill->UseSkill();

}

void SkillMgr::UseRightSkill()
{
	Skill* rightSkill = equipedSkillList[SkillEvents::Right];
	rightSkill->UseSkill();

}

void SkillMgr::UseSpaceSkill()
{
	Skill* spaceSkill = equipedSkillList[SkillEvents::Space];
	spaceSkill->UseSkill();

}

void SkillMgr::LoadExistSkill()
{
	for (auto table : DATATABLE_MGR.Get<SkillTable>(DataTable::Ids::Skill)->GetTable()[(int)SkillDatas::Exist])
	{
		Skill* skill = new Skill(); 
		skill->SetSkillInfo(DATATABLE_MGR.Get<SkillTable>(DataTable::Ids::Skill)->Get((int)table.first, SkillDatas::Exist));
		skill->Init();
		existSkillList[skill->GetSkillId()] = skill;
	}
}

void SkillMgr::LoadBuyedSkill()
{
	for (auto table : DATATABLE_MGR.Get<SkillTable>(DataTable::Ids::Skill)->GetTable()[(int)SkillDatas::Buyed])
	{
		Skill* skill = new Skill();
		skill->SetSkillInfo(DATATABLE_MGR.Get<SkillTable>(DataTable::Ids::Skill)->Get((int)table.first, SkillDatas::Buyed));
		skill->Init();
		buyedSkillList[skill->GetSkillId()] = skill;
	}
}

void SkillMgr::LoadEquipedSkill()
{
	for (auto table : DATATABLE_MGR.Get<SkillTable>(DataTable::Ids::Skill)->GetTable()[(int)SkillDatas::Equiped])
	{
		Skill* skill = new Skill();
		skill->SetSkillInfo(DATATABLE_MGR.Get<SkillTable>(DataTable::Ids::Skill)->Get((int)table.first, SkillDatas::Equiped));
		skill->Init();
		equipedSkillList[skill->GetSkillEvent()] = skill;
	}
}

void SkillMgr::SaveExistedSkill()
{
	rapidcsv::Document doc;
	doc.Clear();

	std::string fileName = "tables/ExistedSkillInfo.csv";

	doc.SetColumnName(0, "Id");
	doc.SetColumnName(1, "SkillNames");
	doc.SetColumnName(2, "ElementTypes / -1: None / 0: Fire / 1: Water / 2: Thunder / 3: Earth / 4: Wind /");
	doc.SetColumnName(3, "SkillTypes / -1: None / 0: Melee / 1: Range / 2: Petrol /");
	doc.SetColumnName(4, "RangeTypes / -1: None / 0: Straight / 1: Curve /");
	doc.SetColumnName(5, "EventTypes / -1: None / 0: Left / 1: Right / 2: Space / 3: Q /");
	doc.SetColumnName(6, "PlayerActions / -1: None / 0: Hand / 1: Kick / 2: Jump / 3: Slam / 4: Focus / 5: JumpKick / 6: JumpSlam /");

	doc.SetColumnName(7, "Damages");
	doc.SetColumnName(8, "ComboDamages");
	doc.SetColumnName(9, "ComboMaxCounts");
	doc.SetColumnName(10, "ComboDurations");
	doc.SetColumnName(11, "MaxSkillChages");
	doc.SetColumnName(12, "ShotCounts");

	doc.SetColumnName(13, "Speeds");
	doc.SetColumnName(14, "Ranges");
	doc.SetColumnName(15, "ExplosionRanges");
	doc.SetColumnName(16, "Amplitudes");
	doc.SetColumnName(17, "Frequencys");
	doc.SetColumnName(18, "DelayDurations");
	doc.SetColumnName(19, "DamageDelays");
	doc.SetColumnName(20, "CoolTimes");
	doc.SetColumnName(21, "RotateSpeeds");

	doc.SetColumnName(22, "IsPenetratings");
	doc.SetColumnName(23, "CanMoveDuringSkills");

	std::vector<int> skillIds;
	std::vector<std::string> skillNames;
	std::vector<int> elementTypes;
	std::vector<int> skillTypes;
	std::vector<int> rangeTypes;
	std::vector<int> eventTypes;
	std::vector<int> playerActions;

	std::vector<int> damages;
	std::vector<int> comboDamages;
	std::vector<int> comboMaxCounts;
	std::vector<float> comboDurations;
	std::vector<int> maxSkillChages;
	std::vector<int> shotCounts;

	std::vector<float> speeds;
	std::vector<float> ranges;
	std::vector<float> explosionRanges;
	std::vector<float> amplitudes;
	std::vector<float> frequencys;
	std::vector<float> delayDurations;
	std::vector<float> damageDelays;
	std::vector<float> coolTimes;
	std::vector<float> rotateSpeeds;

	std::vector<int> isPenetratings;
	std::vector<int> canMoveDuringSkills;

	for (auto skill : existSkillList)
	{
		SkillInfo skillInfo = skill.second->GetSkillInfo();
		skillIds.push_back(skillInfo.skillId);
		skillNames.push_back(skillInfo.spellinfo.skillName);
		elementTypes.push_back((int)skillInfo.elementType);
		skillTypes.push_back((int)skillInfo.skillType);
		rangeTypes.push_back((int)skillInfo.rangeType);
		eventTypes.push_back((int)skillInfo.evnetType);
		playerActions.push_back((int)skillInfo.playerAction);

		damages.push_back(skillInfo.spellinfo.damage);
		comboDamages.push_back(skillInfo.spellinfo.comboDamage);
		comboMaxCounts.push_back(skillInfo.spellinfo.comboMaxCount);
		comboDurations.push_back(skillInfo.spellinfo.comboDuration);
		maxSkillChages.push_back(skillInfo.spellinfo.maxSkillCharge);
		shotCounts.push_back(skillInfo.spellinfo.shotCount);

		speeds.push_back(skillInfo.spellinfo.speed);
		ranges.push_back(skillInfo.spellinfo.range);
		explosionRanges.push_back(skillInfo.spellinfo.explosionRange);
		amplitudes.push_back(skillInfo.spellinfo.amplitude);
		frequencys.push_back(skillInfo.spellinfo.frquency);
		delayDurations.push_back(skillInfo.spellinfo.delayDuration);
		damageDelays.push_back(skillInfo.spellinfo.damageDelay);
		coolTimes.push_back(skillInfo.spellinfo.coolTime);
		rotateSpeeds.push_back(skillInfo.spellinfo.rotateSpeed);

		isPenetratings.push_back((int)skillInfo.spellinfo.isPenetrating);
		canMoveDuringSkills.push_back((int)skillInfo.spellinfo.canMoveDuringSkill);

	}
	doc.SetColumn<int>("Id", skillIds);
	doc.SetColumn<std::string>("SkillNames", skillNames);

	doc.SetColumn<int>("ElementTypes / -1: None / 0: Fire / 1: Water / 2: Thunder / 3: Earth / 4: Wind /", elementTypes);
	doc.SetColumn<int>("SkillTypes / -1: None / 0: Melee / 1: Range / 2: Petrol /", skillTypes);
	doc.SetColumn<int>("RangeTypes / -1: None / 0: Straight / 1: Curve /", rangeTypes);
	doc.SetColumn<int>("EventTypes / -1: None / 0: Left / 1: Right / 2: Space / 3: Q /", eventTypes);
	doc.SetColumn<int>("PlayerActions / -1: None / 0: Hand / 1: Kick / 2: Jump / 3: Slam / 4: Focus / 5: JumpKick / 6: JumpSlam /", playerActions);

	doc.SetColumn<int>("Damages", damages);
	doc.SetColumn<int>("ComboDamages", comboDamages);
	doc.SetColumn<int>("ComboMaxCounts", comboMaxCounts);
	doc.SetColumn<float>("ComboDurations", comboDurations);
	doc.SetColumn<int>("MaxSkillChages", maxSkillChages);
	doc.SetColumn<int>("ShotCounts", shotCounts);

	doc.SetColumn<float>("Speeds", speeds);
	doc.SetColumn<float>("Ranges", ranges);
	doc.SetColumn<float>("ExplosionRanges", explosionRanges);
	doc.SetColumn<float>("Amplitudes", amplitudes);
	doc.SetColumn<float>("Frequencys", frequencys);
	doc.SetColumn<float>("DelayDurations", delayDurations);
	doc.SetColumn<float>("DamageDelays", damageDelays);
	doc.SetColumn<float>("CoolTimes", coolTimes);
	doc.SetColumn<float>("RotateSpeeds", rotateSpeeds);

	doc.SetColumn<int>("IsPenetratings", isPenetratings);
	doc.SetColumn<int>("CanMoveDuringSkills", canMoveDuringSkills);

	doc.Save(fileName);
}

void SkillMgr::SaveBuyedSkill()
{
	rapidcsv::Document doc;
	doc.Clear();

	std::string fileName = "tables/BuyedSkillInfo.csv";

	doc.SetColumnName(0, "Id");
	doc.SetColumnName(1, "SkillNames");
	doc.SetColumnName(2, "ElementTypes / -1: None / 0: Fire / 1: Water / 2: Thunder / 3: Earth / 4: Wind /");
	doc.SetColumnName(3, "SkillTypes / -1: None / 0: Melee / 1: Range / 2: Petrol /");
	doc.SetColumnName(4, "RangeTypes / -1: None / 0: Straight / 1: Curve /");
	doc.SetColumnName(5, "EventTypes / -1: None / 0: Left / 1: Right / 2: Space / 3: Q /");
	doc.SetColumnName(6, "PlayerActions / -1: None / 0: Hand / 1: Kick / 2: Jump / 3: Slam / 4: Focus / 5: JumpKick / 6: JumpSlam /");

	doc.SetColumnName(7, "Damages");
	doc.SetColumnName(8, "ComboDamages");
	doc.SetColumnName(9, "ComboMaxCounts");
	doc.SetColumnName(10, "ComboDurations");
	doc.SetColumnName(11, "MaxSkillChages");
	doc.SetColumnName(12, "ShotCounts");

	doc.SetColumnName(13, "Speeds");
	doc.SetColumnName(14, "Ranges");
	doc.SetColumnName(15, "ExplosionRanges");
	doc.SetColumnName(16, "Amplitudes");
	doc.SetColumnName(17, "Frequencys");
	doc.SetColumnName(18, "DelayDurations");
	doc.SetColumnName(19, "DamageDelays");
	doc.SetColumnName(20, "CoolTimes");
	doc.SetColumnName(21, "RotateSpeeds");

	doc.SetColumnName(22, "IsPenetratings");
	doc.SetColumnName(23, "CanMoveDuringSkills");

	std::vector<int> skillIds;
	std::vector<std::string> skillNames;
	std::vector<int> elementTypes;
	std::vector<int> skillTypes;
	std::vector<int> rangeTypes;
	std::vector<int> eventTypes;
	std::vector<int> playerActions;

	std::vector<int> damages;
	std::vector<int> comboDamages;
	std::vector<int> comboMaxCounts;
	std::vector<float> comboDurations;
	std::vector<int> maxSkillChages;
	std::vector<int> shotCounts;

	std::vector<float> speeds;
	std::vector<float> ranges;
	std::vector<float> explosionRanges;
	std::vector<float> amplitudes;
	std::vector<float> frequencys;
	std::vector<float> delayDurations;
	std::vector<float> damageDelays;
	std::vector<float> coolTimes;
	std::vector<float> rotateSpeeds;

	std::vector<int> isPenetratings;
	std::vector<int> canMoveDuringSkills;

	for (auto skill : buyedSkillList)
	{
		SkillInfo skillInfo = skill.second->GetSkillInfo();
		skillIds.push_back(skillInfo.skillId);
		skillNames.push_back(skillInfo.spellinfo.skillName);
		elementTypes.push_back((int)skillInfo.elementType);
		skillTypes.push_back((int)skillInfo.skillType);
		rangeTypes.push_back((int)skillInfo.rangeType);
		eventTypes.push_back((int)skillInfo.evnetType);
		playerActions.push_back((int)skillInfo.playerAction);

		damages.push_back(skillInfo.spellinfo.damage);
		comboDamages.push_back(skillInfo.spellinfo.comboDamage);
		comboMaxCounts.push_back(skillInfo.spellinfo.comboMaxCount);
		comboDurations.push_back(skillInfo.spellinfo.comboDuration);
		maxSkillChages.push_back(skillInfo.spellinfo.maxSkillCharge);
		shotCounts.push_back(skillInfo.spellinfo.shotCount);

		speeds.push_back(skillInfo.spellinfo.speed);
		ranges.push_back(skillInfo.spellinfo.range);
		explosionRanges.push_back(skillInfo.spellinfo.explosionRange);
		amplitudes.push_back(skillInfo.spellinfo.amplitude);
		frequencys.push_back(skillInfo.spellinfo.frquency);
		delayDurations.push_back(skillInfo.spellinfo.delayDuration);
		damageDelays.push_back(skillInfo.spellinfo.damageDelay);
		coolTimes.push_back(skillInfo.spellinfo.coolTime);
		rotateSpeeds.push_back(skillInfo.spellinfo.rotateSpeed);

		isPenetratings.push_back((int)skillInfo.spellinfo.isPenetrating);
		canMoveDuringSkills.push_back((int)skillInfo.spellinfo.canMoveDuringSkill);
	}
	doc.SetColumn<int>("Id", skillIds);
	doc.SetColumn<std::string>("SkillNames", skillNames);

	doc.SetColumn<int>("ElementTypes / -1: None / 0: Fire / 1: Water / 2: Thunder / 3: Earth / 4: Wind /", elementTypes);
	doc.SetColumn<int>("SkillTypes / -1: None / 0: Melee / 1: Range / 2: Petrol /", skillTypes);
	doc.SetColumn<int>("RangeTypes / -1: None / 0: Straight / 1: Curve /", rangeTypes);
	doc.SetColumn<int>("EventTypes / -1: None / 0: Left / 1: Right / 2: Space / 3: Q /", eventTypes);
	doc.SetColumn<int>("PlayerActions / -1: None / 0: Hand / 1: Kick / 2: Jump / 3: Slam / 4: Focus / 5: JumpKick / 6: JumpSlam /", playerActions);

	doc.SetColumn<int>("Damages", damages);
	doc.SetColumn<int>("ComboDamages", comboDamages);
	doc.SetColumn<int>("ComboMaxCounts", comboMaxCounts);
	doc.SetColumn<float>("ComboDurations", comboDurations);
	doc.SetColumn<int>("MaxSkillChages", maxSkillChages);
	doc.SetColumn<int>("ShotCounts", shotCounts);

	doc.SetColumn<float>("Speeds", speeds);
	doc.SetColumn<float>("Ranges", ranges);
	doc.SetColumn<float>("ExplosionRanges", explosionRanges);
	doc.SetColumn<float>("Amplitudes", amplitudes);
	doc.SetColumn<float>("Frequencys", frequencys);
	doc.SetColumn<float>("DelayDurations", delayDurations);
	doc.SetColumn<float>("DamageDelays", damageDelays);
	doc.SetColumn<float>("CoolTimes", coolTimes);
	doc.SetColumn<float>("RotateSpeeds", rotateSpeeds);

	doc.SetColumn<int>("IsPenetratings", isPenetratings);
	doc.SetColumn<int>("CanMoveDuringSkills", canMoveDuringSkills);

	doc.Save(fileName);
}

void SkillMgr::SaveEquipedSkill()
{
	rapidcsv::Document doc;
	doc.Clear();

	std::string fileName = "tables/EquipedSkillInfo.csv";

	doc.SetColumnName(0, "Id");
	doc.SetColumnName(1, "SkillNames");
	doc.SetColumnName(2, "ElementTypes / -1: None / 0: Fire / 1: Water / 2: Thunder / 3: Earth / 4: Wind /");
	doc.SetColumnName(3, "SkillTypes / -1: None / 0: Melee / 1: Range / 2: Petrol /");
	doc.SetColumnName(4, "RangeTypes / -1: None / 0: Straight / 1: Curve /");
	doc.SetColumnName(5, "EventTypes / -1: None / 0: Left / 1: Right / 2: Space / 3: Q /");
	doc.SetColumnName(6, "PlayerActions / -1: None / 0: Hand / 1: Kick / 2: Jump / 3: Slam / 4: Focus / 5: JumpKick / 6: JumpSlam /");

	doc.SetColumnName(7, "Damages");
	doc.SetColumnName(8, "ComboDamages");
	doc.SetColumnName(9, "ComboMaxCounts");
	doc.SetColumnName(10, "ComboDurations");
	doc.SetColumnName(11, "MaxSkillChages");
	doc.SetColumnName(12, "ShotCounts");

	doc.SetColumnName(13, "Speeds");
	doc.SetColumnName(14, "Ranges");
	doc.SetColumnName(15, "ExplosionRanges");
	doc.SetColumnName(16, "Amplitudes");
	doc.SetColumnName(17, "Frequencys");
	doc.SetColumnName(18, "DelayDurations");
	doc.SetColumnName(19, "DamageDelays");
	doc.SetColumnName(20, "CoolTimes");
	doc.SetColumnName(21, "RotateSpeeds");

	doc.SetColumnName(22, "IsPenetratings");
	doc.SetColumnName(23, "CanMoveDuringSkills");

	std::vector<int> skillIds;
	std::vector<std::string> skillNames;
	std::vector<int> elementTypes;
	std::vector<int> skillTypes;
	std::vector<int> rangeTypes;
	std::vector<int> eventTypes;
	std::vector<int> playerActions;

	std::vector<int> damages;
	std::vector<int> comboDamages;
	std::vector<int> comboMaxCounts;
	std::vector<float> comboDurations;
	std::vector<int> maxSkillChages;
	std::vector<int> shotCounts;

	std::vector<float> speeds;
	std::vector<float> ranges;
	std::vector<float> explosionRanges;
	std::vector<float> amplitudes;
	std::vector<float> frequencys;
	std::vector<float> delayDurations;
	std::vector<float> damageDelays;
	std::vector<float> coolTimes;
	std::vector<float> rotateSpeeds;

	std::vector<int> isPenetratings;
	std::vector<int> canMoveDuringSkills;

	for (auto skill : equipedSkillList)
	{
		SkillInfo skillInfo = skill.second->GetSkillInfo();
		skillIds.push_back(skillInfo.skillId);
		skillNames.push_back(skillInfo.spellinfo.skillName);
		elementTypes.push_back((int)skillInfo.elementType);
		skillTypes.push_back((int)skillInfo.skillType);
		rangeTypes.push_back((int)skillInfo.rangeType);
		eventTypes.push_back((int)skillInfo.evnetType);
		playerActions.push_back((int)skillInfo.playerAction);

		damages.push_back(skillInfo.spellinfo.damage);
		comboDamages.push_back(skillInfo.spellinfo.comboDamage);
		comboMaxCounts.push_back(skillInfo.spellinfo.comboMaxCount);
		comboDurations.push_back(skillInfo.spellinfo.comboDuration);
		maxSkillChages.push_back(skillInfo.spellinfo.maxSkillCharge);
		shotCounts.push_back(skillInfo.spellinfo.shotCount);

		speeds.push_back(skillInfo.spellinfo.speed);
		ranges.push_back(skillInfo.spellinfo.range);
		explosionRanges.push_back(skillInfo.spellinfo.explosionRange);
		amplitudes.push_back(skillInfo.spellinfo.amplitude);
		frequencys.push_back(skillInfo.spellinfo.frquency);
		delayDurations.push_back(skillInfo.spellinfo.delayDuration);
		damageDelays.push_back(skillInfo.spellinfo.damageDelay);
		coolTimes.push_back(skillInfo.spellinfo.coolTime);
		rotateSpeeds.push_back(skillInfo.spellinfo.rotateSpeed);

		isPenetratings.push_back((int)skillInfo.spellinfo.isPenetrating);
		canMoveDuringSkills.push_back((int)skillInfo.spellinfo.canMoveDuringSkill);
	}
	doc.SetColumn<int>("Id", skillIds);
	doc.SetColumn<std::string>("SkillNames", skillNames);

	doc.SetColumn<int>("ElementTypes / -1: None / 0: Fire / 1: Water / 2: Thunder / 3: Earth / 4: Wind /", elementTypes);
	doc.SetColumn<int>("SkillTypes / -1: None / 0: Melee / 1: Range / 2: Petrol /", skillTypes);
	doc.SetColumn<int>("RangeTypes / -1: None / 0: Straight / 1: Curve /", rangeTypes);
	doc.SetColumn<int>("EventTypes / -1: None / 0: Left / 1: Right / 2: Space / 3: Q /", eventTypes);
	doc.SetColumn<int>("PlayerActions / -1: None / 0: Hand / 1: Kick / 2: Jump / 3: Slam / 4: Focus / 5: JumpKick / 6: JumpSlam /", playerActions);

	doc.SetColumn<int>("Damages", damages);
	doc.SetColumn<int>("ComboDamages", comboDamages);
	doc.SetColumn<int>("ComboMaxCounts", comboMaxCounts);
	doc.SetColumn<float>("ComboDurations", comboDurations);
	doc.SetColumn<int>("MaxSkillChages", maxSkillChages);
	doc.SetColumn<int>("ShotCounts", shotCounts);

	doc.SetColumn<float>("Speeds", speeds);
	doc.SetColumn<float>("Ranges", ranges);
	doc.SetColumn<float>("ExplosionRanges", explosionRanges);
	doc.SetColumn<float>("Amplitudes", amplitudes);
	doc.SetColumn<float>("Frequencys", frequencys);
	doc.SetColumn<float>("DelayDurations", delayDurations);
	doc.SetColumn<float>("DamageDelays", damageDelays);
	doc.SetColumn<float>("CoolTimes", coolTimes);
	doc.SetColumn<float>("RotateSpeeds", rotateSpeeds);

	doc.SetColumn<int>("IsPenetratings", isPenetratings);
	doc.SetColumn<int>("CanMoveDuringSkills", canMoveDuringSkills);

	doc.Save(fileName);
}

void SkillMgr::SwapSkill(SkillEvents sEvent, Skill* equipSkill)
{
  	Skill* changeSlot = equipedSkillList[sEvent];
	changeSlot->SetSkillEvent(equipSkill->GetSkillEvent());
	equipSkill->SetSkillEvent(sEvent);

	equipedSkillList[equipSkill->GetSkillEvent()] = equipSkill;
	equipedSkillList[changeSlot->GetSkillEvent()] = changeSlot;
}

void SkillMgr::AddSkill(Skill* newSkill)
{
	auto it = existSkillList.find(newSkill->GetSkillId());
	if (it == existSkillList.end())
		existSkillList[newSkill->GetSkillId()] = newSkill;
	else
		std::cout << "is already" << std::endl;
}

void SkillMgr::BuySkill(Skill* newSkill)
{
	auto it = buyedSkillList.find(newSkill->GetSkillId());
	if (it == buyedSkillList.end())
		buyedSkillList[newSkill->GetSkillId()] = newSkill;
	else
		std::cout << "is already" << std::endl;
}

void SkillMgr::EquipSkill(Skill* newSkill)
{
	auto it = equipedSkillList.find(newSkill->GetSkillEvent());
	if(it == equipedSkillList.end())
		equipedSkillList[newSkill->GetSkillEvent()] = newSkill;
	else
		std::cout << "is already" << std::endl;
}

Skill* SkillMgr::SearchExistedSkill(SkillIds id)
{
	return existSkillList[(int)id];
}

Skill* SkillMgr::SearchBuyedSkill(SkillIds id)
{
	return buyedSkillList[(int)id];
}

Skill* SkillMgr::SearchEquipedSkill(SkillIds id)
{
	Skill* skill = buyedSkillList[(int)id];
	return equipedSkillList[skill->GetSkillEvent()];
}

Skill* SkillMgr::SearchSkill(SkillEvents sEvent)
{
	Skill* skill = equipedSkillList[sEvent];
	return skill;
	//return buyedSkillList[skill->GetSkillId()];
}
