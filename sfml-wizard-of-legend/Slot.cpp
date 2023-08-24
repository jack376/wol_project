#include "stdafx.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "SkillMgr.h"

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

	sortLayer = 102;
	sprite.setScale(4, 4);

	if (isUsed)
	{
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
	}

	switch (slotSkillEvent)
	{
	case SkillEvents::Left:
		skillEventIconId = "Left";
		break;
	case SkillEvents::Right:
		skillEventIconId = "Right";
		break;
	case SkillEvents::Space:
		skillEventIconId = "Space";
		break;
	case SkillEvents::Q:
		skillEventIconId = "Q";
		break;
	case SkillEvents::E:
		skillEventIconId = "E";
		break;
	case SkillEvents::R:
		skillEventIconId = "R";
		break;
	case SkillEvents::M:
		skillEventIconId = "M";
		break;
	case SkillEvents::Tab:
		skillEventIconId = "Tab";
		break;
	default:
		skillEventIconId = "";
		break;
	}
	SetSlotEventIcon(skillEventIconId);
}

void Slot::Release()
{
	UIButton::Release();
}

void Slot::Reset()
{
	UIButton::Reset();
	isUsed = false;
	fadeInOutTimer = 0.f;

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
	selectedSlotIcon.SetPosition(p);
	skillEventIcon.SetPosition(p.x, p.y - 60.f);
}

void Slot::SetPosition(float x, float y)
{
	UIButton::SetPosition(x, y);
	currentSkillIcon.SetPosition(x, y);
	skillEventIcon.SetPosition(x, y - 20.f);
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


void Slot::SetSkillIconId(const std::string& id)
{
	skillIconId = id;
}

void Slot::SetSlotEventIcon(std::string eventId)
{
	skillEventIconId = "graphics/UI/CommandEvent/" + eventId + ".png";
	skillEventIcon.sprite.setTexture(*RESOURCE_MGR.GetTexture(skillEventIconId));
	skillEventIcon.sprite.setScale(3, 3);
}

void Slot::OnClickEvent()
{
	if (selectedSlot)
	{
		// 스킬이 바뀌는 부분
		// 스킬 매니저에서 스킬에 해당하는 부분 바꾸기
		std::string tempStr = GetSkillIconId();
		SetSkillIconId(selectedSlot->GetSkillIconId());
		selectedSlot->SetSkillIconId(tempStr);
		SetSkillIcon();
		selectedSlot->SetSkillIcon();

		Skill* selectedSkill = SKILL_MGR.SearchSkill(selectedSlot->slotSkillEvent);
		SKILL_MGR.SwapSkill(slotSkillEvent, selectedSkill);

		selectedSlot->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
		selectedSlot = nullptr;
	}
	else
	{
		sprite.setColor(sf::Color::Green);
		selectedSlot = this;
	}
}

void Slot::SetSkillIcon()
{
	std::string id = "graphics/UI/SkillIcon/" + skillIconId + ".png";
	if (skillIconId.compare("") != 0)
	{
		currentSkillIcon.sprite.setTexture(*RESOURCE_MGR.GetTexture(id));
		sf::IntRect spriteIntRect = sf::IntRect{ 0, 0, (int)currentSkillIcon.sprite.getLocalBounds().width, (int)currentSkillIcon.sprite.getLocalBounds().height };
		currentSkillIcon.sprite.setTextureRect(spriteIntRect);
		currentSkillIcon.sprite.setScale(3, 3);

	}
}

void Slot::OnClickingEvnet()
{
	if(isUsed)
		sprite.setColor(sf::Color::Color(255, 255, 255, 150));
}

void Slot::SetSelectedSlotIcon()
{
	selectedSlotIcon.sprite.setTexture(*RESOURCE_MGR.GetTexture("graphics/UI/selectSlot1.png"));
	selectedSlotIcon.sprite.setScale(4.5, 4.5);
	SetOrigin(origin);
	selectedSlotIcon.SetActive(false);
}

void Slot::SetIsUsed(bool isUsed)
{
	this->isUsed = isUsed;
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
