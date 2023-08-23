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
	SetSelectedSlotIcon();

	sortLayer = 101;
	sprite.setScale(4, 4);

	OnClick = [this]() {
		OnClickEvent();
	};

	OnClicking = [this]() {
		OnClickingEvnet();

	};
	OnEnter = [this]() {
		OnEnterEvent();
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
	if (selectedSlotIcon.GetActive())
	{
		FadeInOutSlotColor(dt);
	}
	else
	{
		fadeInOutTimer = 0.f;
	}
}

void Slot::Draw(sf::RenderWindow& window)
{
	if(selectedSlotIcon.GetActive())
		window.draw(selectedSlotIcon.sprite);
	UIButton::Draw(window);
	window.draw(skillEventIcon.sprite);
	window.draw(currentSkillIcon.sprite);
}

void Slot::SetPosition(const sf::Vector2f& p)
{
	UIButton::SetPosition(p);
	currentSkillIcon.SetPosition(p);
	skillEventIcon.SetPosition(p);
	selectedSlotIcon.SetPosition(p);
}

void Slot::SetPosition(float x, float y)
{
	UIButton::SetPosition(x, y);
	currentSkillIcon.SetPosition(x, y);
	skillEventIcon.SetPosition(x, y);
	selectedSlotIcon.SetPosition(x, y);

}

void Slot::SetOrigin(Origins origin)
{
	UIButton::SetOrigin(origin);
	currentSkillIcon.SetOrigin(origin);
	skillEventIcon.SetOrigin(origin);
	selectedSlotIcon.SetOrigin(origin);

}

void Slot::SetOrigin(float x, float y)
{
	UIButton::SetOrigin(x, y);
	currentSkillIcon.SetOrigin(x, y);
	skillEventIcon.SetOrigin(x, y);
	selectedSlotIcon.SetOrigin(x, y);

}

void Slot::SetString(const std::string& str)
{
	iconId = str;
}

void Slot::OnClickEvent()
{
	//sprite.setColor(sf::Color::Color(255, 255, 255, 255));
	if (selectedSlot)
	{
		// 바뀌는 부분 적용
		std::string tempStr = GetString();
		SetSkillIcon(selectedSlot->GetString());
		selectedSlot->SetSkillIcon(tempStr);
		selectedSlot->sprite.setColor(sf::Color::Color(255, 255, 255, 255));

		selectedSlot = nullptr;
		std::cout << "Slot Deselected" << std::endl;
	}
	else
	{
		sprite.setColor(sf::Color::Green);
		selectedSlot = this;
		std::cout << "Slot Selected" << std::endl;
	}

}

void Slot::SetSkillIcon(std::string skillIconId)
{
	iconId = skillIconId;
	currentSkillIcon.sprite.setTexture(*RESOURCE_MGR.GetTexture(iconId));
	currentSkillIcon.sprite.setScale(4, 4);
}

void Slot::OnClickingEvnet()
{
	//std::cout << skillEventStr << std::endl;
	sprite.setColor(sf::Color::Color(255, 255, 255, 150));
}

void Slot::SetSelectedSlotIcon()
{
	selectedSlotIcon.sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/UI/selectSlot1.png"));
	selectedSlotIcon.sprite.setScale(4.5, 4.5);
	SetOrigin(origin);
	selectedSlotIcon.SetActive(false);
}

void Slot::OnExitEvent()
{
	if (!selectedSlot)
		sprite.setColor(sf::Color::Color(255, 255, 255, 255));
	selectedSlotIcon.SetActive(false);
}

void Slot::FadeInOutSlotColor(float dt)
{
	fadeInOutTimer += dt;

	if (fadeInOutTimer > fadeInOutDuration)
	{
		fadeInOutTimer = 0.f;
	}

	float oscillation = std::sin(fadeInOutTimer / fadeInOutDuration * (2 * M_PI)); // Range: -1 to 1
	float normalizedOscillation = (oscillation + 1.f) / 2.f; // Normalize to 0 to 1
	sf::Color currentColor = selectedSlotIcon.sprite.getColor();
	sf::Color fadeInOutColor = currentColor;
	fadeInOutColor.a = static_cast<sf::Uint8>(255 * normalizedOscillation); // Use normalized oscillation as alpha
	selectedSlotIcon.sprite.setColor(fadeInOutColor);
}

void Slot::OnEnterEvent()
{
	SetSelectedSlotIcon();
	selectedSlotIcon.SetActive(true);

}
