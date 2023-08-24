#include "stdafx.h"
#include "MenuInventory.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "SkillMgr.h"

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

	
	panel.setTexture(*RESOURCE_MGR.GetTexture("graphics/UI/Panel.png"));
	SetPosition(windowSize.x * 0.3f,windowSize.y * 0.5f);
	SetOrigin(Origins::MC);
	panel.setScale(6, 8);

	slotKey.push_back("Left");
	slotKey.push_back("Right");
	slotKey.push_back("Space");
	slotKey.push_back("Q");
	slotKey.push_back("E");
	slotKey.push_back("R");



	//slot.SetSkillIconId();

	sortLayer = 101;
}

void MenuInventory::Release()
{
	
}

void MenuInventory::Reset()
{
	for (int i = 0; i < slotKey.size(); i++)
	{
		Slot* slot = (Slot*)SCENE_MGR.GetCurrScene()->AddGo(new Slot("graphics/UI/slot1.png"));
		slot->SetPosition((370 + i * 85), 350);
		slot->SetSlotEvent((SkillEvents)i);
		slot->SetOrigin(Origins::MC);
		slot->Init();
		slotList[slotKey[i]] = slot;
	}
	for (int i = 0; i < (int)SkillIds::Count; i++)
	{
		slotList[slotKey[i]]->SetSkillIconId(SKILL_MGR.SearchSkill(slotList[slotKey[i]]->GetSlotEvent())->GetSkillIconId());
		std::cout << SKILL_MGR.SearchSkill(slotList[slotKey[i]]->GetSlotEvent())->GetSkillIconId() << std::endl;
		Skill* temp = SKILL_MGR.SearchSkill(slotList[slotKey[i]]->GetSlotEvent());
		std::string str = SKILL_MGR.SearchSkill(slotList[slotKey[i]]->GetSlotEvent())->GetSkillIconId();
		slotList[slotKey[i]]->SetSkillIcon();
	}
}

void MenuInventory::Update(float dt)
{

}

void MenuInventory::Draw(sf::RenderWindow& window)
{
	window.draw(panel);
}
