#include "stdafx.h"
#include "MenuInventory.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "SkillMgr.h"
#include "InputMgr.h"

MenuInventory::MenuInventory(const std::string& n) : GameObject(n)
{
}

MenuInventory::~MenuInventory()
{
}

void MenuInventory::SetPosition(const sf::Vector2f& p)
{
	position = p;
	panel.setPosition(p);
}

void MenuInventory::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	panel.setPosition({ x,y });
}

void MenuInventory::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(panel, origin);
	}
}

void MenuInventory::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	panel.setOrigin(x, y);
}

void MenuInventory::SetOrigin(sf::RectangleShape& rect, Origins origin)
{
	GameObject::SetOrigin(origin);
	//if (this->origin != Origins::CUSTOM)
	//{
	//	Utils::SetOrigin(rect, origin);
	//}
}

void MenuInventory::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();

	
	panel.setTexture(*RESOURCE_MGR.GetTexture("graphics/UI/ItemInfoPanel.png"));
	SetPosition(windowSize.x * 0.3f,windowSize.y * 0.4f);
	SetOrigin(Origins::MC);
	panel.setScale(6, 8);

	slotKey.push_back("Left");
	slotKey.push_back("Space");
	slotKey.push_back("Right");
	slotKey.push_back("Q");
	slotKey.push_back("E");
	slotKey.push_back("R");



	//slot.SetSkillIconId();

	sortLayer = 105;
}

void MenuInventory::Release()
{
	for (auto& pair : slotList)
	{
		Slot* slot = pair.second;
		SCENE_MGR.GetCurrScene()->RemoveGo(slot);
		delete slot;
	}
	slotList.clear();
}

void MenuInventory::Reset()
{
	for (int i = 0; i < slotKey.size(); i++)
	{
		Slot* slot = (Slot*)SCENE_MGR.GetCurrScene()->AddGo(new Slot("graphics/UI/slot1.png"));
		slot->SetPosition((370 + i * 85), 350);
		slot->SetSlotEvent((SkillEvents)i);
		slot->SetOrigin(Origins::MC);
		slot->SetIsUsed(true);
		slot->SetSkill(SKILL_MGR.SearchSkill(slot->GetSlotEvent()));
		slot->SetIsCoolSlot(false);
		slot->Init();
		slot->sortLayer = 106;
		slotList[slotKey[i]] = slot;
	}
	for (int i = 0; i < 6; i++)
	{
		slotList[slotKey[i]]->SetSkillIconId(SKILL_MGR.SearchSkill(slotList[slotKey[i]]->GetSlotEvent())->GetSkillIconId());
		slotList[slotKey[i]]->SetSkillIcon();
		slotList[slotKey[i]]->SetQuickSlot(quickSlot);
		slotList[slotKey[i]]->SetSlotKey(slotKey[i]);
	}

	AllSetActive(false);
}

void MenuInventory::Update(float dt)
{
			
}

void MenuInventory::Draw(sf::RenderWindow& window)
{
	window.draw(panel);
}

void MenuInventory::AllSetActive(bool isActive)
{
	for (auto table : slotList)
	{
		table.second->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
		table.second->SetActive(isActive);
	}
	
	SetActive(isActive);
}
