#pragma once
#include "SpriteGo.h"
#include "ObjectPool.h"
#include "Monster.h"
#include "SkillEditorElementalSpell.h"

struct SpellInfo
{

	std::string skillName;
	std::string playerAction;

	int damage;
	int comboDamage;
	int comboMaxCount;
	int maxSkillCharge;
	int shotCount;

	float speed;
	float range;
	float explosionRange;
	float delayDuration;
	float damageDelay;
	float coolTime;

	float frquency;
	float amplitude;


	float rotateSpeed;

	float animAngle;

	// 관통여부
	// 0, 1 로 인트형 넘기고 받을때 bool
	bool isPenetrating = false;
	bool canMoveDuringSkill = false;

	float petrolDistance;
};

class ElementalSpell;
class SkillEditorElementalSpell;
class Tile;
class Player;
class SkillEditorPlayer;

class Skill : public SpriteGo
{
protected:
	// UI아이콘 텍스쳐 필요?
	// 실제 스킬 애니메이션 클립 어디서 관리할지
	ElementTypes currentElementType = ElementTypes::None;
	SkillTypes currentSkillType = SkillTypes::None;
	RangeTypes currentRangeType = RangeTypes::None;
	SkillEvents currentEventType = SkillEvents::None;
	PlayerActions currentPlayerActionType = PlayerActions::None;

	sf::Keyboard::Key currentKey = sf::Keyboard::Unknown;
	sf::Mouse::Button currentButton = sf::Mouse::None;

	SpriteGo* skillIcon;

	// 최대 발사개수
	int shotCount;
	float coolTime;

	float spreadAngle;

	SpellInfo spellInfo;
	
	// 실제 스킬
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

	void SetSpellInfo(SpellInfo info) { this->spellInfo = info; }
	void InsertAnimId();
};

