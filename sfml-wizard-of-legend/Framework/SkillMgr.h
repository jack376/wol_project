#pragma once
#include "Singleton.h"
#include "Skill.h"

class Skill;
class Monster;
class Player;
class SkillEditorPlayer;

class SkillMgr : public Singleton<SkillMgr>
{
	friend Singleton<SkillMgr>;

protected:
	SkillMgr() = default;
	virtual ~SkillMgr() override = default;

	// AddSkill을 이용해서 저장
	std::unordered_map<int, Skill*> existSkillList;

	// BuySkill을 이용해서 저장
	std::unordered_map<int, Skill*> buyedSkillList;

	// 슬롯에 넣을시 InputSkill을 이용해서 저장
	std::unordered_map<SkillEvents, Skill*> equipedSkillList;


	std::vector<std::vector<Tile*>>* worldTiles = nullptr;
	std::list<Monster*> monsters;
	Player* player;
	SkillEditorPlayer* editorPlayer;

public:
	// 스킬의 이벤트 값 정보
	// 그것을 토대로 스킬 갈아끼는 용도
	
	void Init();

	//void SkillLoad();
	void UseSkill(SkillEvents sEvent);
	void UseEditorSkill(SkillEvents sEvent);
	void UseQSkill();
	void UseLeftSkill();
	void UseRightSkill();
	void UseSpaceSkill();
	
	void LoadExistSkill();
	void LoadBuyedSkill();
	void LoadEquipedSkill();

	void SaveExistedSkill();
	void SaveBuyedSkill();
	void SaveEquipedSkill();

	void SwapSkill(SkillEvents sEvent, Skill* equipSkill);
	void AddSkill(Skill* newSkill);
	void BuySkill(Skill* newSkill);
	void EquipSkill(Skill* newSkill);

	std::unordered_map<int, Skill*>& GetExistSkillList() { return existSkillList; }
	std::unordered_map<int, Skill*>& GetBuyedSkillList() { return buyedSkillList; }
	std::unordered_map<SkillEvents, Skill*>& GetEquipSkillList() { return equipedSkillList; }

	Skill* SearchExistedSkill(SkillIds id);
	Skill* SearchBuyedSkill(SkillIds id);
	Skill* SearchEquipedSkill(SkillIds id);
	Skill* SearchSkill(SkillEvents sEvent);

	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->worldTiles = tiles; }
	void SetMonsterList(std::list<Monster*>& monsters) { this->monsters = monsters; }
	void SetPlayer(Player*& player) { this->player = player; }
	void SetEditorPlayer(SkillEditorPlayer*& player) { this->editorPlayer = player; }

	// 리스트를 넘겨받아 디버그중에서 확인하기 위한 함수
	std::unordered_map<SkillEvents, Skill*>& ForTestDebugSize() { return equipedSkillList; }

};

#define SKILL_MGR (SkillMgr::Instance())