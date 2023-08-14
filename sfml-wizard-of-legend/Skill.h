#pragma once
#include "SpriteGo.h"

class ElementalSpell;

class Skill : public SpriteGo
{
protected:
	// UI아이콘 텍스쳐 필요?
	// 실제 스킬 애니메이션 클립 어디서 관리할지
	SkillEvents currentEventType = SkillEvents::None;
	SkillTypes currentSkillType = SkillTypes::None;
	ElementTypes currentElementType = ElementTypes::None;
	SpriteGo* skillIcon;

	std::string skillIconId;
	
	int damage;
	int comboDamage;
	int maxSkillCharge;
	
	float speed;
	float range;
	float delayDuration;
	float damageDelay;

	float rotateSpeed;

	// 관통여부
	// 0, 1 로 인트형 넘기고 받을때 bool
	bool isPenetrating = false;
	bool canMoveDuringSkill = false;


	// 곡선 표현


	sf::Vector2f dir;
	
	// 실제 스킬
	ElementalSpell* elementSpell;

public:
	Skill(const std::string& textureId = "", const std::string& n = "");
	virtual ~Skill() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

};

