#pragma once
#include "Scene.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "UIButton.h"


class SceneTitle : public Scene
{
protected:
	sf::RectangleShape animBg;

	sf::Sprite bg;
	sf::Sprite title;
	sf::Sprite logo;
	sf::Sprite miniLogo;

	std::vector<std::pair<sf::Text, std::string>> textLangList;

	sf::Text pressKeyInfo;

	UIButton* singlePlay;
	UIButton* multiPlay;
	UIButton* arena;
	UIButton* option;
	UIButton* creator;
	UIButton* quit;

	std::vector<UIButton*> buttonList;

	std::queue<std::function<void(float)>> functionQueue;
	std::function<void(float)> currentFuction;



	bool fadeIn = true;
	bool isAnyKeyDraw = true;
	bool isButtonDraw = false;
	bool isChangeScene = false;

	sf::Vector2f startTitlePos;
	sf::Vector2f destTitlePos;

	float alpha;
	float fadeInOutTimer = 0.f;
	float faedInOutDuration = 1.0f;


	float fadeInOutContinueTimer = 0.f;
	float faedInOutContinueDuration = 2.0f;

	float animBgFadInTimer = 0.f;
	float animBgFadInDuration = 0.5f;



	float tempTimer = 0.f;
	float tempDuration = 1.f;

	bool isFadeInOutFunc = false;
	bool isPressAnyKeyFunc = false;
	bool isPressButtonFunc = false;

	bool isFadeInOutFuccPush = false;
	bool isPressAnyKeyFuncPush = false;
	//bool isPressButtonFuncPush = false;

	bool isFunctionProcessed = true;

public:
	SceneTitle();
	virtual ~SceneTitle() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetRectangleShape(sf::RectangleShape& rect, sf::Vector2f size, sf::Vector2f pos, sf::Color color);
	void SetSprite(sf::Sprite& sprite, const std::string& textureId, sf::Vector2f size, sf::Vector2f pos);
	void AllSetLangText(Languages lang);

	void ButtonInit(UIButton* btn, sf::Vector2f pos, sf::Vector2f size, std::string tablePath, Languages lang);

	void ProcessFuctionQueue(float dt);

	void FadeInOutLogo(float dt);
	void PressAnyKey(float dt);
	//void PressButton(float dt);
};

