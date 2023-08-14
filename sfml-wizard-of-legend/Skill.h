#pragma once
#include "SpriteGo.h"

class ElementalSpell;

class Skill : public SpriteGo
{
protected:
	// UI������ �ؽ��� �ʿ�?
	// ���� ��ų �ִϸ��̼� Ŭ�� ��� ��������
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

	// ���뿩��
	// 0, 1 �� ��Ʈ�� �ѱ�� ������ bool
	bool isPenetrating = false;
	bool canMoveDuringSkill = false;


	// � ǥ��


	sf::Vector2f dir;
	
	// ���� ��ų
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

