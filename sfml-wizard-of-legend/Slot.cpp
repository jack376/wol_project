#include "stdafx.h"
#include "Slot.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "SkillMgr.h"
#include "QuickSlot.h"

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



	coolTimeText.setCharacterSize(25);
	coolTimeText.setFont(*RESOURCE_MGR.GetFont("fonts/NanumSquareB.ttf"));
	coolTimeText.setFillColor(sf::Color::White);

	skillChargeText.setCharacterSize(25);
	skillChargeText.setFont(*RESOURCE_MGR.GetFont("fonts/NanumSquareB.ttf"));
	skillChargeText.setFillColor(sf::Color::White);
	skillChargeText.setOutlineColor(sf::Color::Black);
	skillChargeText.setOutlineThickness(3);
	
	if (!isUsed) 
	{
		coolDownUI.setFillColor(sf::Color::Red);
		coolDownUI.setSize({ 50, 50 });
	}
}

void Slot::Release()
{
	UIButton::Release();
}

void Slot::Reset()
{
	UIButton::Reset();
	//isUsed = false;
	//isCool = false;
	fadeInOutTimer = 0.f;
	coolDownTimer = 0.f;

	if (isCoolSlot)
	{
		coolDownDuration = currentSkill->GetSkillInfo().spellinfo.coolTime;
		maxCharge = currentSkill->GetSkillInfo().spellinfo.maxSkillCharge;
	}
	else
	{
		coolDownDuration = 0;
	}

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

	if (isCoolSlot)
	{
		if (currentSkill->GetCurrentSkillCharge() < currentSkill->GetMaxSkillCharege() && !isCool)
		{
			isCool = true;
			coolDownTimer = coolDownDuration;
		}
	}

	if(isCool)
		coolDownTimer -= dt;

	// 쿨이 돌면
	if (isCoolSlot)
	{
		if (coolDownTimer < 0)
		{
			coolDownTimer = 0;
			isCool = false;
			currentSkill->AddCurrentSkillCharge(1);
			if(currentSkill->GetCurrentSkillCharge() > 0 && currentSkill->GetMaxSkillCharege() != 0)
				currentSkill->SetIsUsed(false);
		}
	}

	if (isCoolSlot)
	{
		if (currentSkill->GetCurrentSkillCharge() < currentSkill->GetMaxSkillCharege()
			&& currentSkill->GetCurrentSkillCharge() > 0)
		{
			std::stringstream skillChargeStr;
			skillChargeStr << currentSkill->GetCurrentSkillCharge();
			std::string temp = skillChargeStr.str();
			skillChargeText.setString(skillChargeStr.str());
			Utils::SetOrigin(skillChargeText, origin);
		}
		else if (currentSkill->GetCurrentSkillCharge() <= 0)
		{
			std::stringstream coolTimeStr;
			coolTimeStr << std::fixed << std::setprecision(1) << coolDownTimer;
			if (coolDownTimer > 0)
			{
				coolTimeText.setString(coolTimeStr.str());
				Utils::SetOrigin(coolTimeText, origin);
			}
		}
	}

}

void Slot::Draw(sf::RenderWindow& window)
{
	if (isCoolSlot)
	{
		if (currentSkill->GetIsUsed())
			window.draw(coolDownUI);
	}
	if(selectedSlotIcon.GetActive())
		window.draw(selectedSlotIcon.sprite);
	UIButton::Draw(window);
	window.draw(skillEventIcon.sprite);
	window.draw(currentSkillIcon.sprite);

	if (isCoolSlot)
	{
		if (currentSkill->GetCurrentSkillCharge() < currentSkill->GetMaxSkillCharege()
			&& currentSkill->GetCurrentSkillCharge() > 0)
		{
			window.draw(skillChargeText);
		}
		else if(currentSkill->GetCurrentSkillCharge() <= 0)
			window.draw(coolTimeText);
	}
}

void Slot::SetPosition(const sf::Vector2f& p)
{
	UIButton::SetPosition(p);
	currentSkillIcon.SetPosition(p);
	selectedSlotIcon.SetPosition(p);
	skillEventIcon.SetPosition(p.x, p.y - 60.f);
	coolDownUI.setPosition(p);
	coolTimeText.setPosition(p.x - 3.f, p.y - 10.f);
	skillChargeText.setPosition(p.x, p.y - 10.f);

}

void Slot::SetPosition(float x, float y)
{
	UIButton::SetPosition(x, y);
	currentSkillIcon.SetPosition(x, y);
	skillEventIcon.SetPosition(x, y - 20.f);
	selectedSlotIcon.SetPosition(x, y);
	coolDownUI.setPosition(x, y);
	coolTimeText.setPosition(x - 3.f, y - 10.f);
	skillChargeText.setPosition(x , y - 10.f);
}

void Slot::SetOrigin(Origins origin)
{
	UIButton::SetOrigin(origin);
	currentSkillIcon.SetOrigin(origin);
	skillEventIcon.SetOrigin(origin);
	selectedSlotIcon.SetOrigin(origin);
	Utils::SetOrigin(coolDownUI, origin);
	Utils::SetOrigin(coolTimeText, origin);
	Utils::SetOrigin(skillChargeText, origin);
}

void Slot::SetOrigin(float x, float y)
{
	UIButton::SetOrigin(x, y);
	currentSkillIcon.SetOrigin(x, y);
	skillEventIcon.SetOrigin(x, y);
	selectedSlotIcon.SetOrigin(x, y);
	coolDownUI.setOrigin(x, y);
	coolTimeText.setOrigin(x, y);
	skillChargeText.setOrigin(x, y);
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

		Skill* selceted1 = SKILL_MGR.SearchSkill(selectedSlot->slotSkillEvent);
		Skill* selceted2 = SKILL_MGR.SearchSkill(slotSkillEvent);

		// 퀵슬롯 변경 부분
		std::string tempQuickStr =  quickSlot->GetSlotList()[slotKey]->GetSkillIconId();
		quickSlot->GetSlotList()[slotKey]->SetSkillIconId(quickSlot->GetSlotList()[selectedSlot->slotKey]->GetSkillIconId());
		quickSlot->GetSlotList()[selectedSlot->slotKey]->SetSkillIconId(tempQuickStr);
		quickSlot->GetSlotList()[slotKey]->SetSkillIcon();
		quickSlot->GetSlotList()[selectedSlot->slotKey]->SetSkillIcon();

		Skill* selectedSlotSkill = SKILL_MGR.SearchSkill(slotSkillEvent);
		Skill* currentSlotSkill = selectedSkill;

		quickSlot->GetSlotList()[selectedSlot->slotKey]->currentSkill = selectedSlotSkill;
		quickSlot->GetSlotList()[slotKey]->currentSkill = currentSlotSkill;
		//quickSlot->GetSlotList()[selectedSlot->slotKey]->currentSkill = SKILL_MGR.SearchSkill(slotSkillEvent);
		//quickSlot->GetSlotList()[slotKey]->currentSkill = selectedSkill;
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
