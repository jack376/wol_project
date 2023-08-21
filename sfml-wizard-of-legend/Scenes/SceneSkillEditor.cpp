#include "stdafx.h"
#include "SceneSkillEditor.h"
#include "Framework.h"
#include "GameObject.h"
#include "SpriteGo.h"
#include "UIButton.h"
#include "TextGo.h"
#include "InputMgr.h"
#include "SkillEditorPlayer.h"
#include "SkillMgr.h"

SceneSkillEditor::SceneSkillEditor() : Scene(SceneId::SkillEditor)
{
}

void SceneSkillEditor::Init()
{
	Release();

	windowSize = FRAMEWORK.GetWindowSize();

	sf::Vector2f nameSize = { 2.5f, 0.3f };
	sf::Vector2f valueSize = { 4.0f, 0.3f };


	skillName = CreateUI("SkillName", uiStandardPosX, uiStandardPosY + offsetY * 0, nameSize);
	skillNameValue = CreateInputField("WindSlash", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 0, valueSize);
	
	elementType = CreateUI("ElementType", uiStandardPosX, uiStandardPosY + offsetY * 1, nameSize);
	elementTypeValue = CreateInputField("Wind", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 1, valueSize);

	skillType = CreateUI("ElementType", uiStandardPosX, uiStandardPosY + offsetY * 2, nameSize);
	skillTypeValue = CreateInputField("Melee", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 2, valueSize);
	
	skillType = CreateUI("RangeType", uiStandardPosX, uiStandardPosY + offsetY * 3, nameSize);
	skillTypeValue = CreateInputField("None", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 3, valueSize);
	
	eventType = CreateUI("EventType", uiStandardPosX, uiStandardPosY + offsetY * 4, nameSize);
	eventTypeValue = CreateInputField("Left", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 4, valueSize);

	playerAction = CreateUI("PlayerAction", uiStandardPosX, uiStandardPosY + offsetY * 5, nameSize);
	playerActionValue = CreateInputField("Jump", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 5, valueSize);

	damage = CreateUI("Damage", uiStandardPosX, uiStandardPosY + offsetY * 6, nameSize);
	damageValue = CreateInputField("10", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 6, valueSize);
	
	comboDamage = CreateUI("ComboDamage", uiStandardPosX, uiStandardPosY + offsetY * 7, nameSize);
	comboDamageValue = CreateInputField("2", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 7, valueSize);
	
	comboMaxCount = CreateUI("ComboMaxCount", uiStandardPosX, uiStandardPosY + offsetY * 8, nameSize);
	comboMaxCountValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 8, valueSize);
	
	comboDuration = CreateUI("ComboDuration", uiStandardPosX, uiStandardPosY + offsetY * 9, nameSize);
	comboDurationValue = CreateInputField("0.6", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 9, valueSize);
	
	maxSkillCharge = CreateUI("MaxSkillCharge", uiStandardPosX, uiStandardPosY + offsetY * 10, nameSize);
	maxSkillChargeValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 10, valueSize);
	
	shotCount = CreateUI("ShotCount", uiStandardPosX, uiStandardPosY + offsetY * 11, nameSize);
	shotCountValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 11, valueSize);
	
	speed = CreateUI("Speed", uiStandardPosX, uiStandardPosY + offsetY * 12, nameSize);
	speedValue = CreateInputField("100", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 12, valueSize);

	range = CreateUI("Range", uiStandardPosX, uiStandardPosY + offsetY * 13, nameSize);
	rangeValue = CreateInputField("500", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 13, valueSize);
	
	explosionRange = CreateUI("ExplosionRange", uiStandardPosX, uiStandardPosY + offsetY * 14, nameSize);
	explosionRangeValue = CreateInputField("400", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 14, valueSize);
	
	amplitude = CreateUI("Amplitude", uiStandardPosX, uiStandardPosY + offsetY * 15, nameSize);
	amplitudeValue = CreateInputField("8", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 15, valueSize);
	
	frequency = CreateUI("Frequency", uiStandardPosX, uiStandardPosY + offsetY * 16, nameSize);
	frequencyValue = CreateInputField("1.2", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 16, valueSize);
	
	delayDuration = CreateUI("DelayDuration", uiStandardPosX, uiStandardPosY + offsetY * 17, nameSize);
	delayDurationValue = CreateInputField("1", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 17, valueSize);
	
	damageDelay = CreateUI("DamageDelay", uiStandardPosX, uiStandardPosY + offsetY * 18, nameSize);
	damageDelayValue = CreateInputField("0", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 18, valueSize);
	
	coolTime = CreateUI("CoolTime", uiStandardPosX, uiStandardPosY + offsetY * 19, nameSize);
	coolTimeValue = CreateInputField("0", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 19, valueSize);
	
	rotateSpeed = CreateUI("RotateSpeed", uiStandardPosX, uiStandardPosY + offsetY * 20, nameSize);
	rotateSpeedValue = CreateInputField("30", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 20, valueSize);

	isPenetrating = CreateUI("IsPenetrating", uiStandardPosX, uiStandardPosY + offsetY * 21, nameSize);
	isPenetratingValue = CreateInputField("True", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 21, valueSize);

	canMoveDuringSkill = CreateUI("CanMoveDuringSkill", uiStandardPosX, uiStandardPosY + offsetY * 22, nameSize);
	canMoveDuringSkillValue = CreateInputField("True", uiStandardPosX + offsetX, uiStandardPosY + offsetY * 22, valueSize);
	
	
	apply = CreateButton("Apply", uiStandardPosX, uiStandardPosY + offsetY * 24, nameSize, [this]() { Apply(); });
	save = CreateButton("Save", uiStandardPosX, uiStandardPosY + offsetY * 25, nameSize, [this]() { Save(); });
	play = CreateButton("Play", uiStandardPosX, uiStandardPosY + offsetY * 26, nameSize, [this]() { Play(); });

	player = (SkillEditorPlayer*)AddGo(new SkillEditorPlayer());
	player->SetPosition(0, 0);
	player->sprite.setScale(4, 4);
	player->SetOrigin(Origins::MC);
	player->sortLayer = 5;


	for (auto go : gameObjects)
	{
		go->Init();
	}
	SKILL_MGR.SetEditorPlayer(player);
	SKILL_MGR.Init();

}

void SceneSkillEditor::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneSkillEditor::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(0, 0);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void SceneSkillEditor::Exit()
{
	Scene::Exit();
}

void SceneSkillEditor::Update(float dt)
{
	Scene::Update(dt);
}

void SceneSkillEditor::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneSkillEditor::Save()
{
	std::cout << "Button Save" << std::endl;
}

void SceneSkillEditor::Apply()
{
	std::cout << "Button Apply" << std::endl;
	ConvertEnumToInt();
	ConvertBoolToInt();
	Skill* skill = nullptr;
	SpellInfo info;
	info.skillName = skillNameValue->text.getString();
	skill->SetElementType((ElementTypes)elemetTypeInt);
	skill->SetSkillType((SkillTypes)skillTypeInt);
	skill->SetRangeType((RangeTypes)rangeTypeInt);
	skill->SetSkillEvent((SkillEvents)eventTypeInt);
	skill->SetPlayerAction((PlayerActions)playerActionInt);
	std::string damageStr = damageValue->text.getString();
	info.damage = std::stoi(damageStr);
	info.comboDamage;
	info.comboMaxCount;
	info.maxSkillCharge;
	info.shotCount;
	info.speed;
	info.range;
	info.explosionRange;
	info.amplitude;
	info.frquency;
	info.delayDuration;
	info.damageDelay;
	info.coolTime;
	info.rotateSpeed;
	info.isPenetrating;
	info.canMoveDuringSkill;


	//skill->SetSpellInfo();
}

void SceneSkillEditor::Play()
{
	std::cout << "Button Play" << std::endl;
}

void SceneSkillEditor::ConvertEnumToInt()
{
	// ElementTypes
	{
		if (elementTypeValue->text.getString() == "Fire")
		{
			elemetTypeInt = 0;
		}
		else if (elementTypeValue->text.getString() == "Water")
		{
			elemetTypeInt = 1;
		}
		else if (elementTypeValue->text.getString() == "Thunder")
		{
			elemetTypeInt = 2;
		}
		else if (elementTypeValue->text.getString() == "Earth")
		{
			elemetTypeInt = 3;
		}
		else if (elementTypeValue->text.getString() == "Wind")
		{
			elemetTypeInt = 4;
		}
		else
		{
			elemetTypeInt = -1;
		}
	}
	// SkillTypes
	{
		if (elementTypeValue->text.getString() == "Melee")
		{
			skillTypeInt = 0;
		}
		else if (elementTypeValue->text.getString() == "Range")
		{
			skillTypeInt = 1;

		}
		else if (elementTypeValue->text.getString() == "Petrol")
		{
			skillTypeInt = 2;
		}
		else
		{
			skillTypeInt = -1;
		}
		
	}
	// RangeTypes
	{
		if (rangeTypeValue->text.getString() == "Straight")
		{
			rangeTypeInt = 0;
		}
		else if (rangeTypeValue->text.getString() == "Curve")
		{
			rangeTypeInt = 1;
		}
		else
		{
			rangeTypeInt = -1;
		}
		
	}
	// SkillEvents
	{
		if (eventTypeValue->text.getString() == "Left")
		{
			eventTypeInt = 0;
		}
		else if (eventTypeValue->text.getString() == "Right")
		{
			eventTypeInt = 1;
		}
		else if (eventTypeValue->text.getString() == "Space")
		{
			eventTypeInt = 2;
		}
		else if (eventTypeValue->text.getString() == "Q")
		{
			eventTypeInt = 3;
		}
		else
		{
			eventTypeInt = -1;
		}
	}
	// PlayerActions
	{
		if (playerActionValue->text.getString() == "Hand")
		{
			playerActionInt = 0;
		}
		else if (playerActionValue->text.getString() == "Kick")
		{
			playerActionInt = 1;
		}
		else if (playerActionValue->text.getString() == "Jump")
		{
			playerActionInt = 2;
		}
		else if (playerActionValue->text.getString() == "Slam")
		{
			playerActionInt = 3;
		}
		else if (playerActionValue->text.getString() == "Focus")
		{
			playerActionInt = 4;
		}
		else if (playerActionValue->text.getString() == "JumpKick")
		{
			playerActionInt = 5;
		}
		else if (playerActionValue->text.getString() == "JumpSlam")
		{
			playerActionInt = 6;
		}
		else
		{
			playerActionInt = -1;
		}
	}
}

void SceneSkillEditor::ConvertBoolToInt()
{
	// isPenetratingValue
	{
		if (isPenetratingValue->text.getString() == "True")
		{
			isPenetratingInt = 1;
		}
		else if (isPenetratingValue->text.getString() == "False")
		{
			isPenetratingInt = 0;
		}
		else
		{
			isPenetratingInt = 0;
		}
	}
	// canMoveDuringSkill
	{
		if (canMoveDuringSkillValue->text.getString() == "True")
		{
			isPenetratingInt = 1;
		}
		else if (canMoveDuringSkillValue->text.getString() == "False")
		{
			isPenetratingInt = 0;
		}
		else
		{
			isPenetratingInt = 0;
		}
	}
}

UIButton* SceneSkillEditor::CreateUI(const std::string& text, float posX, float posY, sf::Vector2f size)
{
	TextGo* buttonText = (TextGo*)AddGo(new TextGo("fonts/NanumSquareEB.ttf"));
	buttonText->sortLayer = 101;
	buttonText->text.setCharacterSize(15);
	buttonText->text.setPosition(posX, posY);
	buttonText->text.setFillColor(sf::Color::White);
	buttonText->text.setString(text);
	buttonText->SetOrigin(Origins::MC);


	UIButton* newButton = (UIButton*)AddGo(new UIButton("graphics/UI/Panel.png"));
	newButton->SetPosition(posX, posY);
	newButton->sprite.setScale(size);
	newButton->sortLayer = 100;
	newButton->SetOrigin(Origins::MC);

	return newButton;
}

UIButton* SceneSkillEditor::CreateButton(const std::string& text, float posX, float posY, sf::Vector2f size, std::function<void()> onClickAction)
{


	TextGo* buttonText = (TextGo*)AddGo(new TextGo("fonts/NanumSquareEB.ttf"));
	buttonText->sortLayer = 101;
	buttonText->text.setCharacterSize(15);
	buttonText->text.setPosition(posX, posY);
	buttonText->text.setFillColor(sf::Color::White);
	buttonText->text.setString(text);
	buttonText->SetOrigin(Origins::MC);


	UIButton* newButton = (UIButton*)AddGo(new UIButton("graphics/UI/Panel.png"));
	newButton->SetPosition(posX, posY);
	newButton->sprite.setScale(size);
	newButton->sortLayer = 100;
	newButton->SetOrigin(Origins::MC);

	newButton->OnEnter = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(64, 64, 128, 255));
	};
	newButton->OnExit = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
	};
	newButton->OnClick = onClickAction;

	return newButton;
}

UIButton* SceneSkillEditor::CreateInputField(const std::string& text, float posX, float posY, sf::Vector2f size)
{
	TextGo* buttonText = (TextGo*)AddGo(new TextGo("fonts/NanumSquareEB.ttf"));
	buttonText->sortLayer = 101;
	buttonText->text.setCharacterSize(15);
	buttonText->text.setPosition(posX, posY);
	buttonText->text.setFillColor(sf::Color::White);
	buttonText->SetOrigin(Origins::MC);


	UIButton* newButton = (UIButton*)AddGo(new UIButton("graphics/UI/Panel.png"));
	newButton->SetPosition(posX, posY);
	newButton->sprite.setScale(size);
	newButton->sortLayer = 100;
	newButton->SetOrigin(Origins::MC);
	newButton->text = buttonText->text;
	newButton->text.setString(text);
	newButton->OnEnter = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(64, 64, 128, 255));
	};
	newButton->OnExit = [newButton]()
	{
		newButton->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
	};

	newButton->OnClick = [newButton, buttonText]()
	{
		newButton->SetIsInput(true);
		buttonText->text.setString("");
	};

	return newButton;
}
