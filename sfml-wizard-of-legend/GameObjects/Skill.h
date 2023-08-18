#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"

struct SpellInfo
{

	std::string skillIconId;
	std::string playerAction;

	int damage;
	int comboDamage;
	int maxSkillCharge;

	float speed;
	float range;
	float explosionRange;
	float damageDelay;

	float frquency;
	float amplitude;


	float rotateSpeed;

	float animAngle;

	// ���뿩��
	// 0, 1 �� ��Ʈ�� �ѱ�� ������ bool
	bool isPenetrating = false;
	bool canMoveDuringSkill = false;

	float petrolDistance;
};

class ElementalSpell;

class Skill : public SpriteGo
{
protected:
	// UI������ �ؽ��� �ʿ�?
	// ���� ��ų �ִϸ��̼� Ŭ�� ��� ��������
	ElementTypes currentElementType = ElementTypes::None;
	SkillTypes currentSkillType = SkillTypes::None;
	RangeTypes currentRangeType = RangeTypes::None;
	SkillEvents currentEventType = SkillEvents::None;

	sf::Keyboard::Key currentKey = sf::Keyboard::Unknown;
	sf::Mouse::Button currentButton = sf::Mouse::None;

	SpriteGo* skillIcon;

	// �ִ� �߻簳��
	int shotCount;
	float coolTime;

	float spreadAngle;

	SpellInfo skillInfo;
	
	// ���� ��ų
	ElementalSpell* elementSpell;
	ObjectPool<ElementalSpell> pool;


public:
	Skill(const std::string& textureId = "", const std::string& n = "");
	virtual ~Skill() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	SkillEvents GetSkillEvent() const { return currentEventType; }
	void SetSkillEvent(SkillEvents sEvent) { currentEventType = sEvent; }
	void SetElementType(ElementTypes type) { currentElementType = type; }
	void SetSkillType(SkillTypes type) { currentSkillType = type; }
	void SetRangeType(RangeTypes type) { currentRangeType = type; }
	void SetPool(ObjectPool<ElementalSpell>& pool) { this->pool = pool; }

	void UseSkill();

	void SetSpellInfo();
	void InsertAnimId();
};

