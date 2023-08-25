#pragma once
#include "Scene.h"
#include "Skill.h"

class TextGo;
class SpriteGo;
class UIButton;
class SkillMgr;
class SkillEditorPlayer;
class Skill;
class ElementalSpell;


class SceneSkillEditor : public Scene
{
protected:
	sf::Vector2f windowSize;

	std::vector<SkillInfo> skillInfos;

	UIButton* skillName;
	UIButton* skillNameValue;
	int skillId;

	UIButton* elementType;
	UIButton* elementTypeValue;
	int elemetTypeInt;

	UIButton* skillType;
	UIButton* skillTypeValue;
	int skillTypeInt;

	UIButton* rangeType;
	UIButton* rangeTypeValue;
	int rangeTypeInt;

	UIButton* eventType;
	UIButton* eventTypeValue;
	int eventTypeInt;

	UIButton* playerAction;
	UIButton* playerActionValue;
	int playerActionInt;

	UIButton* damage;
	UIButton* damageValue;

	UIButton* comboDamage;
	UIButton* comboDamageValue;

	UIButton* comboMaxCount;
	UIButton* comboMaxCountValue;

	UIButton* comboDuration;
	UIButton* comboDurationValue;

	UIButton* maxSkillCharge;
	UIButton* maxSkillChargeValue;

	UIButton* shotCount;
	UIButton* shotCountValue;

	UIButton* speed;
	UIButton* speedValue;

	UIButton* range;
	UIButton* rangeValue;

	UIButton* explosionRange;
	UIButton* explosionRangeValue;

	UIButton* amplitude;
	UIButton* amplitudeValue;

	UIButton* frequency;
	UIButton* frequencyValue;

	UIButton* delayDuration;
	UIButton* delayDurationValue;

	UIButton* damageDelay;
	UIButton* damageDelayValue;

	UIButton* coolTime;
	UIButton* coolTimeValue;

	UIButton* spreadAngle;
	UIButton* spreadAngleValue;

	UIButton* isPenetrating;
	UIButton* isPenetratingValue;
	int isPenetratingInt;

	UIButton* canMoveDuringSkill;
	UIButton* canMoveDuringSkillValue;
	int canMoveDuringSkillInt;

	// 기능 버튼
	UIButton* apply;
	UIButton* save;
	UIButton* play;


	SkillEditorPlayer* player;

	// csv, 
	Skill* currentSkill;

	float blankPos = 32.0f;

	float offsetX = 320;
	float offsetY = 30;

	float uiStandardPosX = 1400;
	float uiStandardPosY = 100;


public:
	SceneSkillEditor();
	virtual ~SceneSkillEditor() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void Save();
	void SaveCSV(std::vector<SkillInfo>& info);
	void Apply();
	void Play();

	void ConvertEnumToInt();
	void ConvertBoolToInt();
	void ConvertNameToId();


	UIButton* CreateUI(const std::string& text, float posX, float posY, sf::Vector2f size);
	UIButton* CreateButton(const std::string& text, float posX, float posY, sf::Vector2f size, std::function<void()> onClickAction);
	UIButton* CreateInputField(const std::string& text, float posX, float posY, sf::Vector2f size);

};

