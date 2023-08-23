#include "stdafx.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"

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
		if (selectedSlot)
		{

			selectedSlot = nullptr;
		}
		else
		{
			selectedSlot = this;
		}
	};

	OnClicking = [this]() {

	};
	OnEnter = [this]() {

	};
	OnExit = [this]() {

	};

	switch (slotSkillEvent)
	{
	case SkillEvents::Left:
		skillEvent = "Left";
		break;
	case SkillEvents::Right:
		skillEvent = "Right";
		break;
	case SkillEvents::Space:
		skillEvent = "Space";
		break;
	case SkillEvents::Q:
		skillEvent = "Q";
		break;
	case SkillEvents::E:
		skillEvent = "E";
		break;
	case SkillEvents::R:
		skillEvent = "R";
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
	//关俊 弊府绰 规过
	//currentSkillIcon = SpriteGo("");
	
	//sf::Texture* tex = RESOURCE_MGR.GetTexture(textureId);
	//if (tex != nullptr)
	//{
	//	currentSkillIcon.sprite.setTexture(*tex);
	//}
	
	//currentSkillIcon.sprite.setTexture();
	//skillEventIcon.sprite.setTexture();
}

void Slot::Update(float dt)
{
	UIButton::Update(dt);

}

void Slot::Draw(sf::RenderWindow& window)
{
	UIButton::Draw(window);
	window.draw(skillEventIcon.sprite);
	if (isUsed)
	{
		window.draw(currentSkillIcon.sprite);
	}
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
