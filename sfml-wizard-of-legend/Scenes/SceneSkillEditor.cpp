#include "stdafx.h"
#include "rapidcsv.h"
#include "SceneSkillEditor.h"
#include "Framework.h"
#include "GameObject.h"
#include "SpriteGo.h"
#include "UIButton.h"
#include "TextGo.h"
#include "InputMgr.h"
#include "SkillEditorPlayer.h"
#include "SkillMgr.h"

SceneSkillEditor::SceneSkillEditor() : Scene(SceneId::SkillEditor)
{
}

void SceneSkillEditor::Init()
{
	Release();

	windowSize = FRAMEWORK.GetWindowSize();

	sf::Vector2f nameSize = { 2.5f, 0.3f };
	sf::Vector2f valueSize = { 4.0f, 0.3f };

	skillName = CreateUI("SkillName", uiStandardPosX, uiStandardPosY + offsetY * 0, nameSize);
	skillNameValue = CreateInputField("WindSlash", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 0, valueSize);
	
	elementType = CreateUI("ElementType", uiStandardPosX, uiStandardPosY + offsetY * 1, nameSize);
	elementTypeValue = CreateInputField("Wind", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 1, valueSize);

	skillType = CreateUI("SkillType", uiStandardPosX, uiStandardPosY + offsetY * 2, nameSize);
	skillTypeValue = CreateInputField("Melee", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 2, valueSize);
	
	rangeType = CreateUI("RangeType", uiStandardPosX, uiStandardPosY + offsetY * 3, nameSize);
	rangeTypeValue = CreateInputField("None", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 3, valueSize);
	
	eventType = CreateUI("EventType", uiStandardPosX, uiStandardPosY + offsetY * 4, nameSize);
	eventTypeValue = CreateInputField("Left", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 4, valueSize);

	playerAction = CreateUI("PlayerAction", uiStandardPosX, uiStandardPosY + offsetY * 5, nameSize);
	playerActionValue = CreateInputField("Jump", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 5, valueSize);

	damage = CreateUI("Damage", uiStandardPosX, uiStandardPosY + offsetY * 6, nameSize);
	damageValue = CreateInputField("10", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 6, valueSize);
	
	comboDamage = CreateUI("ComboDamage", uiStandardPosX, uiStandardPosY + offsetY * 7, nameSize);
	comboDamageValue = CreateInputField("2", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 7, valueSize);
	
	comboMaxCount = CreateUI("ComboMaxCount", uiStandardPosX, uiStandardPosY + offsetY * 8, nameSize);
	comboMaxCountValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 8, valueSize);
	
	comboDuration = CreateUI("ComboDuration", uiStandardPosX, uiStandardPosY + offsetY * 9, nameSize);
	comboDurationValue = CreateInputField("0.6", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 9, valueSize);
	
	maxSkillCharge = CreateUI("MaxSkillCharge", uiStandardPosX, uiStandardPosY + offsetY * 10, nameSize);
	maxSkillChargeValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 10, valueSize);
	
	shotCount = CreateUI("ShotCount", uiStandardPosX, uiStandardPosY + offsetY * 11, nameSize);
	shotCountValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 11, valueSize);
	
	speed = CreateUI("Speed", uiStandardPosX, uiStandardPosY + offsetY * 12, nameSize);
	speedValue = CreateInputField("100", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 12, valueSize);

	range = CreateUI("Range", uiStandardPosX, uiStandardPosY + offsetY * 13, nameSize);
	rangeValue = CreateInputField("500", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 13, valueSize);
	
	explosionRange = CreateUI("ExplosionRange", uiStandardPosX, uiStandardPosY + offsetY * 14, nameSize);
	explosionRangeValue = CreateInputField("400", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 14, valueSize);
	
	amplitude = CreateUI("Amplitude", uiStandardPosX, uiStandardPosY + offsetY * 15, nameSize);
	amplitudeValue = CreateInputField("8", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 15, valueSize);
	
	frequency = CreateUI("Frequency", uiStandardPosX, uiStandardPosY + offsetY * 16, nameSize);
	frequencyValue = CreateInputField("1.2", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 16, valueSize);
	
	delayDuration = CreateUI("DelayDuration", uiStandardPosX, uiStandardPosY + offsetY * 17, nameSize);
	delayDurationValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 17, valueSize);
	
	damageDelay = CreateUI("DamageDelay", uiStandardPosX, uiStandardPosY + offsetY * 18, nameSize);
	damageDelayValue = CreateInputField("0", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 18, valueSize);
	
	coolTime = CreateUI("CoolTime", uiStandardPosX, uiStandardPosY + offsetY * 19, nameSize);
	coolTimeValue = CreateInputField("0", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 19, valueSize);
	
	spreadAngle = CreateUI("SpreadAngle", uiStandardPosX, uiStandardPosY + offsetY * 20, nameSize);
	spreadAngleValue = CreateInputField("30", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 20, valueSize);

	isPenetrating = CreateUI("IsPenetrating", uiStandardPosX, uiStandardPosY + offsetY * 21, nameSize);
	isPenetratingValue = CreateInputField("True", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 21, valueSize);

	canMoveDuringSkill = CreateUI("CanMoveDuringSkill", uiStandardPosX, uiStandardPosY + offsetY * 22, nameSize);
	canMoveDuringSkillValue = CreateInputField("True", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 22, valueSize);
	
	
	apply = CreateButton("Apply", uiStandardPosX, uiStandardPosY + offsetY * 24, nameSize, [this]() { Apply(); });
	save = CreateButton("Save", uiStandardPosX, uiStandardPosY + offsetY * 25, nameSize, [this]() { Save(); });
	play = CreateButton("Play", uiStandardPosX, uiStandardPosY + offsetY * 26, nameSize, [this]() { Play(); });

	player = (SkillEditorPlayer*)AddGo(new SkillEditorPlayer());
	player->SetPosition(0, 0);
	player->sprite.setScale(4, 4);
	player->SetOrigin(Origins::MC);
	player->sortLayer = 5;


	for (auto go : gameObjects)
	{
		go->Init();
	}
	SKILL_MGR.SetEditorPlayer(player);
	//SKILL_MGR.Init();

}

void SceneSkillEditor::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneSkillEditor::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(0, 0);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void SceneSkillEditor::Exit()
{
	Scene::Exit();
}

void SceneSkillEditor::Update(float dt)
{
	Scene::Update(dt);
}

void SceneSkillEditor::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneSkillEditor::Save()
{
	std::cout << "Button Save" << std::endl;

	ConvertEnumToInt();
	ConvertBoolToInt();
	ConvertNameToId();

	Skill* skill = new Skill();
	SpellInfo spellInfo;
	SkillInfo skillInfo;

	spellInfo.skillName = skillNameValue->text.getString();
	skill->SetSkillIconId(skillNameValue->text.getString());
	skill->SetElementType((ElementTypes)elemetTypeInt);
	skill->SetSkillType((SkillTypes)skillTypeInt);
	skill->SetRangeType((RangeTypes)rangeTypeInt);
	skill->SetSkillEvent((SkillEvents)eventTypeInt);
	skill->SetPlayerAction((PlayerActions)playerActionInt);

	std::string damageStr = damageValue->text.getString();
	spellInfo.damage = std::stoi(damageStr);

	std::string comboDamageStr = comboDamageValue->text.getString();
	spellInfo.comboDamage = std::stoi(comboDamageStr);

	std::string comboMaxCountStr = comboMaxCountValue->text.getString();
	spellInfo.comboMaxCount = std::stoi(comboMaxCountStr);

	std::string comboDurationStr = comboDurationValue->text.getString();
	spellInfo.comboDuration = std::stof(comboDurationStr);

	std::string maxSkillChargeStr = maxSkillChargeValue->text.getString();
	spellInfo.maxSkillCharge = std::stoi(maxSkillChargeStr);

	std::string shotCountStr = shotCountValue->text.getString();
	spellInfo.shotCount = std::stoi(shotCountStr);

	std::string speedStr = speedValue->text.getString();
	spellInfo.speed = std::stof(speedStr);

	std::string rangeStr = rangeValue->text.getString();
	spellInfo.range = std::stof(rangeStr);

	std::string explosionRangeStr = explosionRangeValue->text.getString();
	spellInfo.explosionRange = std::stof(explosionRangeStr);

	std::string amplitudeStr = amplitudeValue->text.getString();
	spellInfo.amplitude = std::stof(amplitudeStr);

	std::string frquencyStr = frequencyValue->text.getString();
	spellInfo.frequency = std::stof(frquencyStr);

	std::string delayDurationStr = delayDurationValue->text.getString();
	spellInfo.delayDuration = std::stof(delayDurationStr);

	std::string damageDelayStr = damageDelayValue->text.getString();
	spellInfo.damageDelay = std::stof(damageDelayStr);

	std::string coolTimeStr = coolTimeValue->text.getString();
	spellInfo.coolTime = std::stof(coolTimeStr);

	std::string spreadAngleStr = spreadAngleValue->text.getString();
	spellInfo.spreadAngle = std::stof(spreadAngleStr);

	// bool형 
	spellInfo.isPenetrating = (bool)isPenetratingInt;
	spellInfo.canMoveDuringSkill = (bool)canMoveDuringSkillInt;

	// Save에 필요한가?
	skillInfo.elementType = (ElementTypes)elemetTypeInt;
	skillInfo.skillType = (SkillTypes)skillTypeInt;
	skillInfo.rangeType = (RangeTypes)rangeTypeInt;
	skillInfo.evnetType = (SkillEvents)eventTypeInt;
	skillInfo.playerAction = (PlayerActions)playerActionInt;
	skillInfo.spellinfo = spellInfo;
	skillInfo.skillId = skillId;

	skillInfos.push_back(skillInfo);

	SaveCSV(skillInfos);

	// skillInfo를 넘긴다 그걸 토대로 정보를 skill에 입력하여 대입시킨다.
	// 아래의 부분을 실제 스킬에 적용할 때 해야함
	 
	//skill->SetEditorPlayer(player);
	//skill->SetSkillInfo(skillInfo);
	//skill->SetElementType(skillInfo.elementType);
	//skill->SetSkillType(skillInfo.skillType);
	//skill->SetRangeType(skillInfo.rangeType);
	//skill->SetSkillEvent(skillInfo.evnetType);
	//skill->SetPlayerAction(skillInfo.playerAction);
}

void SceneSkillEditor::SaveCSV(std::vector<SkillInfo>& info)
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
	doc.SetColumnName(21, "SpreadAngles");

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

	for (auto skillInfo : skillInfos)
	{
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
		frequencys.push_back(skillInfo.spellinfo.frequency);
		delayDurations.push_back(skillInfo.spellinfo.delayDuration);
		damageDelays.push_back(skillInfo.spellinfo.damageDelay);
		coolTimes.push_back(skillInfo.spellinfo.coolTime);
		rotateSpeeds.push_back(skillInfo.spellinfo.spreadAngle);

		isPenetratings.push_back(skillInfo.spellinfo.isPenetrating);
		canMoveDuringSkills.push_back(skillInfo.spellinfo.canMoveDuringSkill);
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

void SceneSkillEditor::Apply()
{
	std::cout << "Button Apply" << std::endl;
	ConvertEnumToInt();
	ConvertBoolToInt();
	ConvertNameToId();
	Skill* skill = new Skill();
	SpellInfo spellInfo;
	SkillInfo skillInfo;

	spellInfo.skillName = skillNameValue->text.getString();

	skill->SetElementType((ElementTypes)elemetTypeInt);
	skill->SetSkillType((SkillTypes)skillTypeInt);
	skill->SetRangeType((RangeTypes)rangeTypeInt);
	skill->SetSkillEvent((SkillEvents)eventTypeInt);
	skill->SetPlayerAction((PlayerActions)playerActionInt);

	std::string damageStr = damageValue->text.getString();
	spellInfo.damage = std::stoi(damageStr);

	std::string comboDamageStr = comboDamageValue->text.getString();
	spellInfo.comboDamage = std::stoi(comboDamageStr);

	std::string comboMaxCountStr = comboMaxCountValue->text.getString();
	spellInfo.comboMaxCount = std::stoi(comboMaxCountStr);

	std::string comboDurationStr = comboDurationValue->text.getString();
	spellInfo.comboDuration = std::stof(comboDurationStr);

	std::string maxSkillChargeStr = maxSkillChargeValue->text.getString();
	spellInfo.maxSkillCharge = std::stoi(maxSkillChargeStr);

	std::string shotCountStr = shotCountValue->text.getString();
	spellInfo.shotCount = std::stoi(shotCountStr);

	std::string speedStr = speedValue->text.getString();
	spellInfo.speed = std::stof(speedStr);

	std::string rangeStr = rangeValue->text.getString();
	spellInfo.range = std::stof(rangeStr);

	std::string explosionRangeStr = explosionRangeValue->text.getString();
	spellInfo.explosionRange = std::stof(explosionRangeStr);

	std::string amplitudeStr = amplitudeValue->text.getString();
	spellInfo.amplitude = std::stof(amplitudeStr);

	std::string frquencyStr = frequencyValue->text.getString();
	spellInfo.frequency = std::stof(frquencyStr);

	std::string delayDurationStr = delayDurationValue->text.getString();
	spellInfo.delayDuration = std::stof(delayDurationStr);

	std::string damageDelayStr = damageDelayValue->text.getString();
	spellInfo.damageDelay = std::stof(damageDelayStr);

	std::string coolTimeStr = coolTimeValue->text.getString();
	spellInfo.coolTime = std::stof(coolTimeStr);

	std::string rotateSpeedStr = spreadAngleValue->text.getString();
	spellInfo.spreadAngle = std::stof(rotateSpeedStr);

	// bool형 
	spellInfo.isPenetrating = (bool)isPenetratingInt;
	spellInfo.canMoveDuringSkill = (bool)canMoveDuringSkillInt;


	skillInfo.elementType = (ElementTypes)elemetTypeInt;
	skillInfo.skillType = (SkillTypes)skillTypeInt;
	skillInfo.rangeType = (RangeTypes)rangeTypeInt;
	skillInfo.evnetType = (SkillEvents)eventTypeInt;
	skillInfo.playerAction = (PlayerActions)playerActionInt;
	skillInfo.spellinfo = spellInfo;
	 
	 
	 
	// 아래의 것들은 실제 스킬에 적용해야함
	skill->SetEditorPlayer(player);
	skill->SetSkillInfo(skillInfo);

	skill->SetElementType(skillInfo.elementType);
	skill->SetSkillType(skillInfo.skillType);
	skill->SetRangeType(skillInfo.rangeType);
	skill->SetSkillEvent(skillInfo.evnetType);
	skill->SetPlayerAction(skillInfo.playerAction);

	currentSkill = skill;
}

void SceneSkillEditor::Play()
{
	std::cout << "Button Play" << std::endl;
	currentSkill->UseEditorSkill();
}

void SceneSkillEditor::ConvertEnumToInt()
{
	// ElementTypes
	{
		if (elementTypeValue->text.getString() == "Fire")
		{
			elemetTypeInt = 0;
		}
		else if (elementTypeValue->text.getString() == "Water")
		{
			elemetTypeInt = 1;
		}
		else if (elementTypeValue->text.getString() == "Thunder")
		{
			elemetTypeInt = 2;
		}
		else if (elementTypeValue->text.getString() == "Earth")
		{
			elemetTypeInt = 3;
		}
		else if (elementTypeValue->text.getString() == "Wind")
		{
			elemetTypeInt = 4;
		}
		else
		{
			elemetTypeInt = -1;
		}
	}
	// SkillTypes
	{
		if (skillTypeValue->text.getString() == "Melee")
		{
			skillTypeInt = 0;
		}
		else if (skillTypeValue->text.getString() == "Range")
		{
			skillTypeInt = 1;

		}
		else if (skillTypeValue->text.getString() == "Petrol")
		{
			skillTypeInt = 2;
		}
		else
		{
			skillTypeInt = -1;
		}
		
	}
	// RangeTypes
	{
		if (rangeTypeValue->text.getString() == "Straight")
		{
			rangeTypeInt = 0;
		}
		else if (rangeTypeValue->text.getString() == "Curve")
		{
			rangeTypeInt = 1;
		}
		else
		{
			rangeTypeInt = -1;
		}
		
	}
	// SkillEvents
	{
		if (eventTypeValue->text.getString() == "Left")
		{
			eventTypeInt = 0;
		}
		else if (eventTypeValue->text.getString() == "Right")
		{
			eventTypeInt = 1;
		}
		else if (eventTypeValue->text.getString() == "Space")
		{
			eventTypeInt = 2;
		}
		else if (eventTypeValue->text.getString() == "Q")
		{
			eventTypeInt = 3;
		}
		else if (eventTypeValue->text.getString() == "E")
		{
			eventTypeInt = 4;
		}
		else if (eventTypeValue->text.getString() == "R")
		{
			eventTypeInt = 5;
		}
		else
		{
			eventTypeInt = -1;
		}
	}
	// PlayerActions
	{
		if (playerActionValue->text.getString() == "Hand")
		{
			playerActionInt = 0;
		}
		else if (playerActionValue->text.getString() == "Kick")
		{
			playerActionInt = 1;
		}
		else if (playerActionValue->text.getString() == "Jump")
		{
			playerActionInt = 2;
		}
		else if (playerActionValue->text.getString() == "Slam")
		{
			playerActionInt = 3;
		}
		else if (playerActionValue->text.getString() == "Focus")
		{
			playerActionInt = 4;
		}
		else if (playerActionValue->text.getString() == "HandKick")
		{
			playerActionInt = 5;
		}
		else if (playerActionValue->text.getString() == "JumpKick")
		{
			playerActionInt = 6;
		}
		else if (playerActionValue->text.getString() == "JumpSlam")
		{
			playerActionInt = 7;
		}
		else
		{
			playerActionInt = -1;
		}
	}
}

void SceneSkillEditor::ConvertBoolToInt()
{
	// isPenetratingValue
	{
		if (isPenetratingValue->text.getString() == "True")
		{
			isPenetratingInt = 1;
		}
		else if (isPenetratingValue->text.getString() == "False")
		{
			isPenetratingInt = 0;
		}
		else
		{
			isPenetratingInt = 0;
		}
	}
	// canMoveDuringSkill
	{
		if (canMoveDuringSkillValue->text.getString() == "True")
		{
			isPenetratingInt = 1;
		}
		else if (canMoveDuringSkillValue->text.getString() == "False")
		{
			isPenetratingInt = 0;
		}
		else
		{
			isPenetratingInt = 0;
		}
	}
}

void SceneSkillEditor::ConvertNameToId()
{
	if (skillNameValue->text.getString() == "ExplodingFireball")
	{
		skillId = 0;
	}
	else if (skillNameValue->text.getString() == "WindSlash")
	{
		skillId = 1;
	}
	else if (skillNameValue->text.getString() == "DragonArc")
	{
		skillId = 2;
	}
	else if (skillNameValue->text.getString() == "EarthKnuckles")
	{
		skillId = 3;
	}
	else if (skillNameValue->text.getString() == "FlameCleaver")
	{
		skillId = 4;
	}
	else if (skillNameValue->text.getString() == "FlameStrike")
	{
		skillId = 5;
	}
	else if (skillNameValue->text.getString() == "FrostFan")
	{
		skillId = 6;
	}
	else if (skillNameValue->text.getString() == "GustVolley")
	{
		skillId = 7;
	}
	else if (skillNameValue->text.getString() == "IceDagger")
	{
		skillId = 8;
	}
	else if (skillNameValue->text.getString() == "StoneShot")
	{
		skillId = 9;
	}	
	else if (skillNameValue->text.getString() == "VoltDisc")
	{
		skillId = 10;
	}
	else
	{
		skillId = -1;
	}

}

UIButton* SceneSkillEditor::CreateUI(const std::string& text, float posX, float posY, sf::Vector2f size)
{
	TextGo* buttonText = (TextGo*)AddGo(new TextGo("fonts/NanumSquareEB.ttf"));
	buttonText->sortLayer = 101;
	buttonText->text.setCharacterSize(15);
	buttonText->text.setPosition(posX, posY);
	buttonText->text.setFillColor(sf::Color::White);
	buttonText->text.setString(text);
	buttonText->SetOrigin(Origins::MC);


	UIButton* newButton = (UIButton*)AddGo(new UIButton("graphics/UI/Panel.png"));
	newButton->SetPosition(posX, posY);
	newButton->sprite.setScale(size);
	newButton->sortLayer = 100;
	newButton->SetOrigin(Origins::MC);

	return newButton;
}

UIButton* SceneSkillEditor::CreateButton(const std::string& text, float posX, float posY, sf::Vector2f size, std::function<void()> onClickAction)
{
	TextGo* buttonText = (TextGo*)AddGo(new TextGo("fonts/NanumSquareEB.ttf"));
	buttonText->sortLayer = 101;
	buttonText->text.setCharacterSize(15);
	buttonText->text.setPosition(posX, posY);
	buttonText->text.setFillColor(sf::Color::White);
	buttonText->text.setString(text);
	buttonText->SetOrigin(Origins::MC);


	UIButton* newButton = (UIButton*)AddGo(new UIButton("graphics/UI/Panel.png"));
	newButton->SetPosition(posX, posY);
	newButton->sprite.setScale(size);
	newButton->sortLayer = 100;
	newButton->SetOrigin(Origins::MC);

	newButton->OnEnter = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(64, 64, 128, 255));
	};
	newButton->OnExit = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
	};
	newButton->OnClick = onClickAction;

	return newButton;
}

UIButton* SceneSkillEditor::CreateInputField(const std::string& text, float posX, float posY, sf::Vector2f size)
{
	TextGo* buttonText = (TextGo*)AddGo(new TextGo("fonts/NanumSquareEB.ttf"));
	buttonText->sortLayer = 101;
	buttonText->text.setCharacterSize(15);
	buttonText->text.setPosition(posX, posY);
	buttonText->text.setFillColor(sf::Color::White);
	buttonText->SetOrigin(Origins::MC);


	UIButton* newButton = (UIButton*)AddGo(new UIButton("graphics/UI/Panel.png"));
	newButton->SetPosition(posX, posY);
	newButton->sprite.setScale(size);
	newButton->sortLayer = 100;
	newButton->SetOrigin(Origins::MC);
	newButton->text = buttonText->text;
	newButton->text.setString(text);
	newButton->OnEnter = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(64, 64, 128, 255));
	};
	newButton->OnExit = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
	};

	newButton->OnClick = [newButton, buttonText]()
	{
		newButton->SetIsInput(true);
		buttonText->text.setString("");
	};

	return newButton;
}
