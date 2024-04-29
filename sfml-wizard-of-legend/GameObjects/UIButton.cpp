#include "stdafx.h"
#include "UIButton.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "TextGo.h"

UIButton::UIButton(const std::string& textureId, const std::string& n)
	:SpriteGo(textureId, n)
{
}

UIButton::~UIButton()
{

}

void UIButton::Init()
{
	SpriteGo::Init();
	font.loadFromFile("fonts/NanumSquareEB.ttf");
	text.setFont(font);
	text.setCharacterSize(20);
	text.setFillColor(sf::Color::White);

}

void UIButton::Release()
{
	SpriteGo::Release();
}

void UIButton::Reset()
{
	SpriteGo::Reset();

	isHover = false;
	isPressed = false;
	isInput = false;
}

void UIButton::Update(float dt)
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f uiMousePos = SCENE_MGR.GetCurrScene()->ScreenToUiPos(mousePos);

	bool prevHover = isHover;
	isHover = sprite.getGlobalBounds().contains(uiMousePos);	// 들어가는 순간 지금 isHover는 true 전 프레임은 false

	if (!prevHover && isHover)	// 엔터
	{
		if (OnEnter != nullptr)
			OnEnter();
	}
	if (prevHover && !isHover)	// Exit
	{
		if (OnExit != nullptr)
			OnExit();
	}

	if (isHover && INPUT_MGR.GetMouseButtonDown(sf::Mouse::Left) && !isPressed)
	{
		isPressed = true;
	}
	// 클릭
	if (isHover && INPUT_MGR.GetMouseButtonUp(sf::Mouse::Left) && isPressed)
	{
			if (OnClick != nullptr)
				OnClick();
			isPressed = false;
	}

	// 클릭 중
	if (isHover && INPUT_MGR.GetMouseButton(sf::Mouse::Left) && isPressed)
	{
			if (OnClicking != nullptr)
				OnClicking();
	}

	if (isInput)
	{
		InputString();
	}
}

void UIButton::Draw(sf::RenderWindow& window)
{
	SpriteGo::Draw(window);
	window.draw(text);
}

void UIButton::SetPosition(const sf::Vector2f& p)
{
	SpriteGo::SetPosition(p);
	text.setPosition(p);
}

void UIButton::SetPosition(float x, float y)
{
	SpriteGo::SetPosition(x, y);
	text.setPosition(x, y);
}

void UIButton::SetOrigin(Origins origin)
{
	SpriteGo::SetOrigin(origin);
	Utils::SetOrigin(text, origin);
}

void UIButton::SetOrigin(float x, float y)
{
	SpriteGo::SetOrigin(x, y);
	text.setOrigin(x, y);
}

void UIButton::InputString()
{
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		Reset();

		return;
	}
	else if (!INPUT_MGR.GetTextList().empty())
	{
		if (INPUT_MGR.GetTextList().front() == '\b')
		{
			std::string s = text.getString();
			if (s.empty()) return;
			s.pop_back();
			text.setString(s);
			SetOrigin(origin);
		}
		else
		{
			std::string s = text.getString();
			text.setString(s + INPUT_MGR.GetTextList().front());
			SetOrigin(origin);
		}
	}
}
