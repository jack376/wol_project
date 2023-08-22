#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Monster.h"
#include "SkillEditorElementalSpell.h"

struct SpellInfo
{
	std::string skillName;
	//std::string playerAction;

	int damage;
	int comboDamage;
	int comboMaxCount;
	float comboDuration;
	int maxSkillCharge;
	int shotCount;

	float speed;
	float range;
	float explosionRange;
	float amplitude;
	float frquency;
	float delayDuration;
	float damageDelay;
	float coolTime;
	float rotateSpeed;

	// ���뿩��
	// 0, 1 �� ��Ʈ�� �ѱ�� ������ bool
	bool isPenetrating = false;
	bool canMoveDuringSkill = false;





	//float animAngle;
	//float petrolDistance;
};

struct SkillInfo
{
	int skillId;
	ElementTypes elementType;
	SkillTypes skillType;
	RangeTypes rangeType;
	SkillEvents evnetType;
	PlayerActions playerAction;

	SpellInfo spellinfo;
};
class ElementalSpell;
class SkillEditorElementalSpell;
class Tile;
class Player;
class SkillEditorPlayer;

class Skill : public SpriteGo
{
protected:
	// UI������ �ؽ��� �ʿ�?
	// ���� ��ų �ִϸ��̼� Ŭ�� ��� ��������
	int skillId;
	ElementTypes currentElementType = ElementTypes::None;
	SkillTypes currentSkillType = SkillTypes::None;
	RangeTypes currentRangeType = RangeTypes::None;
	SkillEvents currentEventType = SkillEvents::None;
	PlayerActions currentPlayerActionType = PlayerActions::None;

	sf::Keyboard::Key currentKey = sf::Keyboard::Unknown;
	sf::Mouse::Button currentButton = sf::Mouse::None;

	SpriteGo* skillIcon;

	// �ִ� �߻簳��
	int shotCount;
	float coolTime;

	float spreadAngle;

	SpellInfo spellInfo;
	SkillInfo skillInfo;
	
	// ���� ��ų
	//ElementalSpell* elementSpell;
	ObjectPool<ElementalSpell> pool;
	ObjectPool<SkillEditorElementalSpell> editorPool;

	std::list<Monster*> monsters;
	std::vector<std::vector<Tile*>>* worldTiles = nullptr;

	Player* player;
	SkillEditorPlayer* editorPlayer;

public:
	Skill(const std::string& textureId = "", const std::string& n = "");
	virtual ~Skill() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	SkillEvents GetSkillEvent() const { return currentEventType; }
	int GetSkillId() const { return skillId; }

	void SetSkillEvent(SkillEvents sEvent) { currentEventType = sEvent; }
	void SetElementType(ElementTypes type) { currentElementType = type; }
	void SetSkillType(SkillTypes type) { currentSkillType = type; }
	void SetRangeType(RangeTypes type) { currentRangeType = type; }
	void SetPlayerAction(PlayerActions type) { currentPlayerActionType = type; }
	
	void SetPool(ObjectPool<ElementalSpell>& pool) { this->pool = pool; }
	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->worldTiles = tiles; }
	void SetMonsterList(std::list<Monster*>& monsters) { this->monsters = monsters; }
	void SetPlayer(Player*& player) { this->player = player; }

	void SetEditorPlayer(SkillEditorPlayer*& player) { this->editorPlayer = player; }

	void UseSkill();
	void UseEditorSkill();

	void SetSkillInfo(SkillInfo info) { this->skillInfo = info; }
	void SetSpellInfo(SpellInfo info) { this->skillInfo.spellinfo = info; }
	void InsertAnimId();


};

