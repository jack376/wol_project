#include "stdafx.h"
#include "SceneTitle.h"
#include "GameObject.h"
#include "SpriteGo.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "StringTable.h"
#include "DataTableMgr.h"
#include "UIButton.h"
#include "ResourceMgr.h"


SceneTitle::SceneTitle() : Scene(SceneId::Title)
{

}

void SceneTitle::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	StringTable* stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);

	
	SetRectangleShape(animBg, { 1920, 1080 }, windowSize * 0.5f ,sf::Color::Black);
	sf::Color alpha = animBg.getFillColor();
	alpha.a = 0;
	animBg.setFillColor(alpha);

	SetSprite(bg, "graphics/UI/Title/Background.png", {1, 1}, {windowSize * 0.5f});
	SetSprite(title, "graphics/UI/Title/TitleLogo.png", {1, 1}, { windowSize.x * 0.5f, windowSize.y * 0.6f });
	SetSprite(logo, "graphics/UI/Title/Contingent99Logo.png", {1, 1}, { windowSize * 0.5f });
	SetSprite(miniLogo, "graphics/UI/Title/MiniContingent99.png", {1, 1}, { windowSize * 0.8f });

	singlePlay = (UIButton*)AddGo(new UIButton());
	multiPlay = (UIButton*)AddGo(new UIButton());
	arena = (UIButton*)AddGo(new UIButton());
	option = (UIButton*)AddGo(new UIButton());
	creator = (UIButton*)AddGo(new UIButton());
	quit = (UIButton*)AddGo(new UIButton());

	buttonList.push_back(singlePlay);
	buttonList.push_back(multiPlay);
	buttonList.push_back(arena);
	buttonList.push_back(option);
	buttonList.push_back(creator);
	buttonList.push_back(quit);

	textLangList.push_back(std::make_pair(pressKeyInfo, "PRESS"));

	//textLangList.push_back(std::make_pair(singlePlay->text, "SINGLE"));
	//textLangList.push_back(std::make_pair(multiPlay->text, "MULTI"));
	//textLangList.push_back(std::make_pair(arena->text, "ARENA"));
	//textLangList.push_back(std::make_pair(option->text, "OPTION"));
	//textLangList.push_back(std::make_pair(creator->text, "CREATE"));
	//textLangList.push_back(std::make_pair(quit->text, "QUIT"));

	AllSetLangText(Variables::CurrentLang);
	textLangList[0].first.setPosition(windowSize.x * 0.5f, windowSize.y * 0.85f);

	//textLangList[1].first.setPosition(windowSize.x * 0.5f, windowSize.y * 0.5f);
	//textLangList[2].first.setPosition(windowSize.x * 0.5f, windowSize.y * 0.58f);
	//textLangList[3].first.setPosition(windowSize.x * 0.5f, windowSize.y * 0.66f);
	//textLangList[4].first.setPosition(windowSize.x * 0.5f, windowSize.y * 0.74f);
	//textLangList[5].first.setPosition(windowSize.x * 0.5f, windowSize.y * 0.82f);
	//textLangList[6].first.setPosition(windowSize.x * 0.5f, windowSize.y * 0.9f);

	ButtonInit(singlePlay, { windowSize.x * 0.5f, windowSize.y * 0.5f }, {175, 45}, "SINGLE", Variables::CurrentLang);
	ButtonInit(multiPlay, { windowSize.x * 0.5f, windowSize.y * 0.58f }, {145, 45}, "MULTI", Variables::CurrentLang);
	ButtonInit(arena, { windowSize.x * 0.5f, windowSize.y * 0.66f }, {65, 45}, "ARENA", Variables::CurrentLang);
	ButtonInit(option, { windowSize.x * 0.5f, windowSize.y * 0.74f }, {65, 45}, "OPTION", Variables::CurrentLang);
	ButtonInit(creator, { windowSize.x * 0.5f, windowSize.y * 0.82f }, {100, 45}, "CREATE", Variables::CurrentLang);
	ButtonInit(quit, { windowSize.x * 0.5f, windowSize.y * 0.9f }, {65, 45}, "QUIT", Variables::CurrentLang);

	singlePlay->OnClick = [this]() {
		isChangeScene = true;
	};

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneTitle::Release()
{

	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneTitle::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(0, 0);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);
	Scene::Enter();

}

void SceneTitle::Exit()
{
	Scene::Exit();

}

void SceneTitle::Update(float dt)
{
	Scene::Update(dt);
	if (!isFadeInOutFuccPush)
	{
		functionQueue.push([&](float dt) {
			FadeInOutLogo(dt);
			});
		isFadeInOutFuccPush = true;
	}	
	
	if (!isPressAnyKeyFuncPush)
	{
		functionQueue.push([&](float dt) {
			PressAnyKey(dt);
			});
		isPressAnyKeyFuncPush = true;
	}	
	
	//if (!isPressButtonFuncPush)
	//{
	//	functionQueue.push([&](float dt) {
	//		PressButton(dt);
	//		});
	//	isPressButtonFuncPush = true;
	//}

	if(isFunctionProcessed && !currentFuction)
		ProcessFuctionQueue(dt);
	if(currentFuction)
		currentFuction(dt);

	if (isChangeScene)
	{
		SCENE_MGR.ChangeScene(SceneId::Game);
	}
}

void SceneTitle::Draw(sf::RenderWindow& window)
{

	if (isFadeInOutFunc)
		window.draw(logo);
	if (!isFadeInOutFunc)
	{
		window.draw(bg);
		window.draw(animBg);
		window.draw(miniLogo);
		window.draw(title);

		if (isAnyKeyDraw)
		{
			window.draw(textLangList[0].first);
		}
	}
	if (isButtonDraw)
	{
		Scene::Draw(window);
	}
}
void SceneTitle::SetRectangleShape(sf::RectangleShape& rect, sf::Vector2f size, sf::Vector2f pos, sf::Color color)
{
	rect.setFillColor(color);
	rect.setSize(size);
	rect.setPosition(pos);
	Utils::SetOrigin(rect, Origins::MC);
}

void SceneTitle::SetSprite(sf::Sprite& sprite, const std::string& textureId, sf::Vector2f size, sf::Vector2f pos)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	sprite.setScale(size);
	sprite.setPosition(pos);
	Utils::SetOrigin(sprite, Origins::MC);
}

void SceneTitle::AllSetLangText(Languages lang)
{
	StringTable* stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);
	for (auto& pair : textLangList)
	{

		pair.first.setFont(*RESOURCE_MGR.GetFont("fonts/NanumSquareB.ttf"));
		pair.first.setCharacterSize(30);
		pair.first.setString(stringTable->GetUni(pair.second, lang));
		pair.first.setColor(sf::Color::White);
		pair.first.setOutlineColor(sf::Color::Black);

		sf::Color alpha = pair.first.getColor();
		sf::Color lineAlpha = pair.first.getOutlineColor();
		alpha.a = 170;
		lineAlpha.a = 170;
		pair.first.setColor(alpha);
		pair.first.setOutlineColor(lineAlpha);

		pair.first.setOutlineThickness(2);
		Utils::SetOrigin(pair.first, Origins::MC);
	}

}


void SceneTitle::ButtonInit(UIButton* btn, sf::Vector2f pos, sf::Vector2f size , std::string tablePath, Languages lang)
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();

	btn->sprite.setScale(size);
	btn->SetPosition(pos);
	btn->SetOrigin(Origins::MC);
	   


	StringTable* stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);

	btn->text.setFont(*RESOURCE_MGR.GetFont("fonts/NanumSquareB.ttf"));
	btn->text.setCharacterSize(30);
	btn->text.setString(stringTable->GetUni(tablePath, lang));
	btn->text.setFillColor(sf::Color::White);
	btn->text.setOutlineColor(sf::Color::Black);

	sf::Color alpha = btn->text.getColor();
	sf::Color lineAlpha = btn->text.getOutlineColor();
	alpha.a = 100;
	lineAlpha.a = 100;
	btn->text.setFillColor(alpha);
	btn->text.setOutlineColor(lineAlpha);
	btn->text.setOutlineThickness(2);

	Utils::SetOrigin(btn->text, Origins::MC);

	btn->OnEnter = [this, btn]() {
		btn->text.setFillColor(sf::Color::Color(255, 255, 255, 255));
		btn->text.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
		std::cout << "OnEnter" << std::endl;
	};
	btn->OnExit = [this, btn]() {
		btn->text.setFillColor(sf::Color::Color(255, 255, 255, 170));
		btn->text.setOutlineColor(sf::Color::Color(0, 0, 0, 170));
		std::cout << "OnExit" << std::endl;

	};
	btn->OnClick = [this, btn]() {
		btn->text.setFillColor(sf::Color::Color(255, 255, 255, 255));
		btn->text.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
		std::cout << "OnClick" << std::endl;

	};	
}

void SceneTitle::ProcessFuctionQueue(float dt)
{
	if (!functionQueue.empty())
	{
		currentFuction = functionQueue.front();
		functionQueue.pop();
		isFunctionProcessed = false;
	}
}

void SceneTitle::FadeInOutLogo(float dt)
{	
	isFadeInOutFunc = true;
	float ratio = Utils::Clamp(fadeInOutTimer / faedInOutDuration, 0.f, 1.f);

	if (ratio >= 1.f && fadeIn)
	{
		fadeIn = false;
	}

	if (fadeIn)
	{
		fadeInOutTimer += dt;
		alpha = Utils::Lerp(0, 255, ratio);
	}
	else
	{
		fadeInOutTimer -= dt;
		alpha = Utils::Lerp(0, 255, ratio);
	}
	logo.setColor(sf::Color(255, 255, 255, alpha));

	std::cout << ratio << std::endl;

	if (!fadeIn && ratio <= 0.f)
	{
		isFunctionProcessed = true;
		currentFuction = nullptr;
		isFadeInOutFunc = false;
	}
}

void SceneTitle::PressAnyKey(float dt)
{

	// 어떤 입력이 들어오든
	if (INPUT_MGR.GetEvent().type == sf::Event::KeyReleased 
		|| INPUT_MGR.GetEvent().type == sf::Event::MouseButtonPressed)
	{
		isAnyKeyDraw = false;

	}

	fadeInOutContinueTimer += dt;
	if (fadeInOutContinueTimer > faedInOutContinueDuration)
	{
		fadeInOutContinueTimer = 0.f;
	}
	float oscillation = std::sin(fadeInOutContinueTimer / faedInOutContinueDuration * (2 * M_PI)); // Range: -1 to 1
	float normalizedOscillation = (oscillation + 1.f) / 2.f; // Normalize to 0 to 1
	sf::Color currentColor = textLangList[0].first.getColor();
	sf::Color currentLineColor = textLangList[0].first.getOutlineColor();

	sf::Color fadeInOutColor = currentColor;
	sf::Color fadeInOutLinfColor = currentLineColor;

	fadeInOutColor.a = static_cast<sf::Uint8>(255 * normalizedOscillation); // Use normalized oscillation as alpha
	fadeInOutLinfColor.a = static_cast<sf::Uint8>(255 * normalizedOscillation); // Use normalized oscillation as alpha
	textLangList[0].first.setColor(fadeInOutColor);
	textLangList[0].first.setOutlineColor(fadeInOutLinfColor);

	//std::cout << animBgFadInTimer << std::endl;
	if (!isAnyKeyDraw && animBgFadInTimer <= animBgFadInDuration)
	{
		animBgFadInTimer += dt;
		float ratio = Utils::Clamp(animBgFadInTimer / animBgFadInDuration, 0.f, 1.f);
		alpha = Utils::Lerp(0, 160, ratio);
		sf::Color fadeInColor = animBg.getFillColor();
		fadeInColor.a = alpha;
		animBg.setFillColor(fadeInColor);

		startTitlePos = title.getPosition();
		destTitlePos = sf::Vector2f(FRAMEWORK.GetWindowSize().x * 0.5f, FRAMEWORK.GetWindowSize().y * 0.35f);
		float ratioPos = Utils::Clamp(animBgFadInTimer / animBgFadInDuration, 0.f, 1.f);
		sf::Vector2f titlePos = Utils::Lerp(startTitlePos, destTitlePos, ratio);
		title.setPosition(titlePos);
	}

	if (animBgFadInTimer >= animBgFadInDuration)
	{
		isButtonDraw = true;
	}

	if (tempTimer > tempDuration)
	{
		isFunctionProcessed = true;
		currentFuction = nullptr;
		isPressAnyKeyFunc = false;
	}
	isPressAnyKeyFunc = true;


	//std::cout << "PressAnyKey Func Processed" << std::endl;
}

//void SceneTitle::PressButton(float dt)
//{
//	//std::cout << "PressButton Func Processed" << std::endl;
//	isPressButtonFunc = true;
//	
//	//isFunctionProcessed = true;
//	//currentFuction = nullptr;
//}
