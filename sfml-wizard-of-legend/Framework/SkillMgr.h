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

	// Ű �Է�

	// ��ų �����Ϳ��� ���̺�� InputSkill�� �̿��ؼ� ����
	std::unordered_map<SkillEvents, Skill*> skillList;


	std::vector<std::vector<Tile*>>* worldTiles = nullptr;
	std::list<Monster*> monsters;
	Player* player;
	SkillEditorPlayer* editorPlayer;
public:
	// ��ų�� �̺�Ʈ �� ����
	// �װ��� ���� ��ų ���Ƴ��� �뵵
	
	void Init();

	//void SkillLoad();
	void UseSkill(SkillEvents sEvent);
	void UseEditorSkill(SkillEvents sEvent);
	void UseQSkill();
	void UseLeftSkill();
	void UseRightSkill();
	void UseSpaceSkill();
	void SwapSkill(SkillEvents sEvent, Skill* equipSkill);
	void InputSkill(Skill* newSkill);

	void SetTiles(std::vector<std::vector<Tile*>>* tiles) { this->worldTiles = tiles; }
	void SetMonsterList(std::list<Monster*>& monsters) { this->monsters = monsters; }
	void SetPlayer(Player*& player) { this->player = player; }
	void SetEditorPlayer(SkillEditorPlayer*& player) { this->editorPlayer = player; }

	// ����Ʈ�� �Ѱܹ޾� ������߿��� Ȯ���ϱ� ���� �Լ�
	std::unordered_map<SkillEvents, Skill*>& ForTestDebugSize() { return skillList; }

};

#define SKILL_MGR (SkillMgr::Instance())