#include "stdafx.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"

Slot* Slot::selectedSlot = nullptr;

Slot::Slot(const std::string& textureId, const std::string& n)
	: UIButton(textureId, n)
{

}

Slot::~Slot()
{
}

void Slot::Init()
{
	UIButton::Init();
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	SetPosition(position);
	sortLayer = 101;
	sprite.setScale(4, 4);

	OnClick = [this]() {
		OnClickEvent();
	};

	OnClicking = [this]() {
		OnClickingEvnet();

	};
	OnEnter = [this]() {

	};
	OnExit = [this]() {
		OnExitEvent();
	};

	switch (slotSkillEvent)
	{
	case SkillEvents::Left:
		iconId = "Left";
		break;
	case SkillEvents::Right:
		iconId = "Right";
		break;
	case SkillEvents::Space:
		iconId = "Space";
		break;
	case SkillEvents::Q:
		iconId = "Q";
		break;
	case SkillEvents::E:
		iconId = "E";
		break;
	case SkillEvents::R:
		iconId = "R";
		break;
	}
}

void Slot::Release()
{
	UIButton::Release();

}

void Slot::Reset()
{
	UIButton::Reset();
	isUsed = false;

}

void Slot::Update(float dt)
{
	UIButton::Update(dt);

}

void Slot::Draw(sf::RenderWindow& window)
{
	UIButton::Draw(window);
	window.draw(skillEventIcon.sprite);

		window.draw(currentSkillIcon.sprite);
}

void Slot::SetPosition(const sf::Vector2f& p)
{
	UIButton::SetPosition(p);
	currentSkillIcon.SetPosition(p);
	skillEventIcon.SetPosition(p);

}

void Slot::SetPosition(float x, float y)
{
	UIButton::SetPosition(x, y);
	currentSkillIcon.SetPosition(x, y);
	skillEventIcon.SetPosition(x, y);

}

void Slot::SetOrigin(Origins origin)
{
	UIButton::SetOrigin(origin);
	currentSkillIcon.SetOrigin(origin);
	skillEventIcon.SetOrigin(origin);

}

void Slot::SetOrigin(float x, float y)
{
	UIButton::SetOrigin(x, y);
	currentSkillIcon.SetOrigin(x, y);
	skillEventIcon.SetOrigin(x, y);

}

void Slot::SetString(const std::string& str)
{
	iconId = str;
}

void Slot::OnClickEvent()
{
	sprite.setColor(sf::Color::Color(255, 255, 255, 255));
	if (selectedSlot)
	{
		//currentSkillIcon.sprite = selectedSlot->currentSkillIcon.sprite;
		std::string tempStr = GetString();
		SetSkillIcon(selectedSlot->GetString());
		selectedSlot->SetSkillIcon(tempStr);

		selectedSlot = nullptr;
		std::cout << "Slot Deselected" << std::endl;
	}
	else
	{
		selectedSlot = this;
		std::cout << "Slot Selected" << std::endl;
	}

}

void Slot::SetSkillIcon(std::string skillIconId)
{
	//关俊 弊府绰 规过
	//currentSkillIcon = SpriteGo("");
	
	//sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	//if (tex != nullptr)
	//{
	//	currentSkillIcon.sprite.setTexture(*tex);
	//}
	
	//currentSkillIcon.sprite.setTexture();
	//skillEventIcon.sprite.setTexture();
	
	iconId = skillIconId;
	currentSkillIcon.sprite.setTexture(*RESOURCE_MGR.GetTexture(iconId));
	currentSkillIcon.sprite.setScale(4, 4);
}

void Slot::OnClickingEvnet()
{
	//std::cout << skillEventStr << std::endl;
	sprite.setColor(sf::Color::Color(255, 255, 255, 150));
}

void Slot::OnExitEvent()
{
	sprite.setColor(sf::Color::Color(255, 255, 255, 255));

}
