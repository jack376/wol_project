#pragma once
#include "UIButton.h"

class SpriteGo;
class Skill;

class Slot : public UIButton
{
protected:
	static Slot* selectedSlot;

	Skill* currentSkill;

	// 스킬 아이디를 이용해서 Sprite 이미지 변경

	SkillEvents slotSkillEvent = SkillEvents::None;
	SkillIds skillId = SkillIds::None;

	SpriteGo currentSkillIcon;
	SpriteGo selectedSlotIcon;

	SpriteGo skillEventIcon;

	std::string skillIconId;
	std::string skillEventIconId;

	bool isUsed = false;

	float fadeInOutTimer = 0.f;
	float fadeInOutDuration = 1.0f;


public:
	Slot(const std::string& textureId = "", const std::string& n = "");
	virtual ~Slot() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	SkillEvents GetSlotEvent() { return slotSkillEvent; }

	void SetSlotEvent(SkillEvents sEvent) { slotSkillEvent = sEvent; }
	void SetSlotEventIcon(const std::string eventId);
	void SetSkillIconId(const std::string& id);
	void SetSkillIcon();

	void SetSelectedSlotIcon();
	void FadeInOutSlotColor(float dt);

	std::string GetSkillIconId() { return skillIconId; }

	void OnClickEvent();
	void OnClickingEvnet();
	void OnEnterEvent();
	void OnExitEvent();

 };