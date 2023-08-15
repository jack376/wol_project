#pragma once
#include "SpriteGo.h"

struct SpellInfo
{

	std::string skillIconId;
	std::string playerAction;

	int damage;
	int comboDamage;
	int maxSkillCharge;
	int shotCount;

	float speed;
	float range;
	float explosionRange;
	float delayDuration;
	float damageDelay;
	float coolTime;

	float rotateSpeed;

	// ���뿩��
	// 0, 1 �� ��Ʈ�� �ѱ�� ������ bool
	bool isPenetrating = false;
	bool canMoveDuringSkill = false;


	// � ǥ��


	sf::Vector2f dir;

	sf::Vector2f startPos;
	sf::Vector2f destPos;

};

class ElementalSpell;

class Skill : public SpriteGo
{
protected:
	// UI������ �ؽ��� �ʿ�?
	// ���� ��ų �ִϸ��̼� Ŭ�� ��� ��������
	SkillEvents currentEventType = SkillEvents::None;
	SkillTypes currentSkillType = SkillTypes::None;
	ElementTypes currentElementType = ElementTypes::None;

	sf::Keyboard::Key currentKey = sf::Keyboard::Unknown;

	SpriteGo* skillIcon;

	SpellInfo skillInfo;
	
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

	void SetSpellInfo();
};

