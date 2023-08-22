#pragma once
#include "SpriteGo.h"

class TextGo;

class UIButton : public SpriteGo
{
protected:
	bool isHover = false;
	bool isPressed = false;

	bool isInput = false;

	sf::Font font;
public:
	int storageNum;

	int price = 0;

	bool isLock = false;
	bool isSkinLock = false;

	UIButton(const std::string& textureId = "", const std::string& n = "");
	virtual ~UIButton() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	void SetIsInput(bool isInput) { this->isInput = isInput; }

	void InputString();

	sf::Text text;

	std::function<void()> OnClick;
	std::function<void()> OnClicking;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;
};

