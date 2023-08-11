#pragma once
#include "GameObject.h"

class BaseUI : public GameObject
{
protected:
	UiType uiType = UiType::Text;

	bool isHover = false;
	unsigned int fontSize = 54;

	float buttonSizeX = 0.0f;
	float buttonSizeY = 0.0f;
	float textHeight = 0.0f;

	sf::Color textColor;

	sf::RectangleShape body;
	sf::Color bodyColor;

	std::vector<sf::Sprite> cornerStrokes;
	sf::Sprite cornerStroke;

	std::vector<sf::RectangleShape> sides;
	sf::RectangleShape side;

	sf::Color strokeColor;

	std::vector<sf::Sprite> corners;
	sf::Sprite corner;

	std::vector<sf::Sprite> sideStrokes;
	sf::Sprite sideStroke;

	sf::Vector2f buttonModifySize;
	sf::Vector2f buttonModifyPosition;

	sf::RectangleShape buttonCollider;

public:	
	sf::Text text;

	std::function<void()> OnClick;
	std::function<void()> OnEnter;
	std::function<void()> OnExit;

	BaseUI(const std::string& n = "", UiType uiType = UiType::Text);
	virtual ~BaseUI() override;

	virtual void Init() override;
	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetColor(sf::Color color = sf::Color::Black);
	void SetTextColor(sf::Color color = sf::Color::White);
	void SetStrokeColor(sf::Color color = sf::Color::Transparent);

	void SetString(const std::string& string);
	void SetCharacterSize(unsigned int fontSize = 54);

	void SetSizeAdd(float x, float y);
	void SetPosition(float x, float y);
};

