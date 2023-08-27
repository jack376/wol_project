#pragma once
#include "GameObject.h"

class TextGo;

class GameResult : public GameObject
{
protected:
	sf::Shader shader;
	sf::Texture paletteTexture;

	sf::RectangleShape panel;
	sf::RectangleShape upLine;
	sf::RectangleShape downLine;

	sf::Sprite spaceButtonIcon;
	sf::Sprite exitButtonIcon;	
	sf::Sprite playerDeadIcon;

	sf::Sprite leftIcon;
	sf::Sprite spaceIcon;
	sf::Sprite rightIcon;
	sf::Sprite qIcon;
	sf::Sprite eIcon;
	sf::Sprite rIcon;
	sf::Sprite mIcon;


	std::vector<std::pair<sf::Text, std::string>> textLangList;
	std::vector<sf::Text> textValueList;

	sf::Text resultText;
	sf::Text gemText;
	sf::Text totalTimeText;
	sf::Text stageText;
	sf::Text continueText;
	sf::Text exitText;

	sf::Text gemValueText;
	sf::Text totalTimeValueText;
	sf::Text stageValueText;


public:
	GameResult(const std::string& n = "");
	virtual ~GameResult() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);
	virtual void SetOrigin(sf::RectangleShape& rect, Origins origin);

	void SetSprite(sf::Sprite& sprite, const std::string& textureId, sf::Vector2f size, sf::Vector2f pos);
	void SetRectangleShape(sf::RectangleShape& rect, sf::Vector2f size, sf::Vector2f pos, sf::Color color);

	void AllSetLangText(Languages lang);
	void AllSetActive(bool isActive);

};

