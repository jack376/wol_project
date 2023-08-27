#include "stdafx.h"
#include "QuickSlot.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "SkillMgr.h"
#include "InputMgr.h"

QuickSlot::QuickSlot(const std::string& n) : GameObject(n)
{
}

QuickSlot::~QuickSlot()
{
}

void QuickSlot::SetPosition(const sf::Vector2f& p)
{
	position = p;
}

void QuickSlot::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void QuickSlot::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
	}
}

void QuickSlot::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
}

void QuickSlot::SetOrigin(sf::RectangleShape& rect, Origins origin)
{
	GameObject::SetOrigin(origin);
	//if (this->origin != Origins::CUSTOM)
	//{
	//	Utils::SetOrigin(rect, origin);
	//}
}

void QuickSlot::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();


	SetPosition(windowSize.x * 0.05f, windowSize.y * 0.95f);
	SetOrigin(Origins::MC);

	slotKey.push_back("Left");
	slotKey.push_back("Space");
	slotKey.push_back("Right");
	slotKey.push_back("Q");
	slotKey.push_back("E");
	slotKey.push_back("R");
	slotKey.push_back("Tab");
	slotKey.push_back("M");



	//slot.SetSkillIconId();

	sortLayer = 101;
}

void QuickSlot::Release()
{
	for (auto& pair : slotList)
	{
		Slot* slot = pair.second;
		SCENE_MGR.GetCurrScene()->RemoveGo(slot);
		delete slot;
	}
	slotList.clear();
}

void QuickSlot::Reset()
{
	for (int i = 0; i < slotKey.size(); i++)
	{
		Slot* slot = (Slot*)SCENE_MGR.GetCurrScene()->AddGo(new Slot("graphics/UI/slot1.png"));
		slot->SetPosition((100 + i * 70), 900);
		slot->SetSlotEvent((SkillEvents)i);
		slot->SetOrigin(Origins::MC);
		slot->SetIsUsed(false);
		Skill* skill = SKILL_MGR.SearchSkill(slot->GetSlotEvent());
		slot->SetSkill(SKILL_MGR.SearchSkill(slot->GetSlotEvent()));
		Skill* tmep = slot->GetSkill();
		slot->SetIsCoolSlot(true);
		slot->Init();
		slotList[slotKey[i]] = slot;
	}
	for (int i = 0; i < 6; i++)
	{
		slotList[slotKey[i]]->SetSkillIconId(SKILL_MGR.SearchSkill(slotList[slotKey[i]]->GetSlotEvent())->GetSkillIconId());		
		slotList[slotKey[i]]->SetSkillIcon();
	}

	slotList["Tab"]->sprite.setScale(3.5, 3.5);
	slotList["Tab"]->SetPosition(550, 900);
	slotList["Tab"]->SetSkillIcon();
	slotList["Tab"]->SetIsCoolSlot(false);

	slotList["M"]->sprite.setScale(3.5, 3.5);
	slotList["M"]->SetPosition(620, 900);
	slotList["M"]->SetSkillIcon();
	slotList["M"]->SetIsCoolSlot(false);

	//SetActive(false);
}

void QuickSlot::Update(float dt)
{

}

void QuickSlot::Draw(sf::RenderWindow& window)
{
}

void QuickSlot::AllSetActive(bool isActive)
{
	for (auto table : slotList)
	{
		table.second->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
		table.second->SetActive(isActive);
	}

	SetActive(isActive);
}
