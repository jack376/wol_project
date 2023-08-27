#include "stdafx.h"
#include "GameResult.h"
#include "Framework.h"
#include "StringTable.h"
#include "DataTable.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "SceneGame.h"

GameResult::GameResult(const std::string& n)
{
}

GameResult::~GameResult()
{
}

void GameResult::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	SetPosition(windowSize * 0.5f);

	float yOffset = 15.0f / 31.0f;
	paletteTexture.loadFromFile("shader/WizardPalette.png");
	shader.loadFromFile("shader/FragShader.frag", sf::Shader::Fragment);
	shader.setUniform("paletteTexture", paletteTexture);
	shader.setUniform("paletteYOffset", yOffset);

	panel.setFillColor(sf::Color::Black);
	panel.setSize(windowSize);

	SetRectangleShape(upLine, {640, 5}, {390, 60}, sf::Color::White);
	SetRectangleShape(downLine, { 640, 5 }, { 390, 150 }, sf::Color::White);


	SetSprite(spaceButtonIcon, "graphics/UI/Result/Space.png", { 3, 3 }, { 1270, 775 });
	SetSprite(exitButtonIcon, "graphics/UI/Result/Escape.png", { 3, 3 }, { 1270, 825 });
	SetSprite(playerDeadIcon, "graphics/UI/Result/WizardDeadIcon.png", { 4, 4 }, { 850, 100 });
	
	SetSprite(leftIcon, "graphics/UI/Result/Arcana.png", { 4, 4 }, { 970, 100 });
	SetSprite(spaceIcon, "graphics/UI/Result/Arcana.png", { 4, 4 }, { 1055, 100 });
	SetSprite(rightIcon, "graphics/UI/Result/Arcana.png", { 4, 4 }, { 1140, 100 });
	SetSprite(qIcon, "graphics/UI/Result/Arcana.png", { 4, 4 }, { 1225, 100 });
	SetSprite(eIcon, "graphics/UI/Result/Arcana.png", { 4, 4 }, { 1310, 100 });
	SetSprite(rIcon, "graphics/UI/Result/Arcana.png", { 4, 4 }, { 1395, 100 });
	SetSprite(mIcon, "graphics/UI/Result/Arcana.png", { 3, 3 }, { 1470, 100 });

	SetOrigin(Origins::MC);

	sortLayer = 110;

	textLangList.push_back(std::make_pair(resultText, "RESULT"));
	textLangList.push_back(std::make_pair(stageText, "STAGE"));
	textLangList.push_back(std::make_pair(gemText, "GEM"));
	textLangList.push_back(std::make_pair(totalTimeText, "TOTALTIME"));
	textLangList.push_back(std::make_pair(continueText, "CONTINUE"));
	textLangList.push_back(std::make_pair(exitText, "EXIT"));


	textValueList.push_back(gemValueText);
	textValueList.push_back(totalTimeValueText);
	textValueList.push_back(stageValueText);

	// 위치는 다르게
	AllSetLangText(Variables::CurrentLang);

	textLangList[0].first.setPosition(350, 100);
	textLangList[1].first.setPosition(70, 180);
	textLangList[2].first.setPosition(70, 230);
	textLangList[3].first.setPosition(70, 280);

	textLangList[4].first.setPosition(1300, 770);
	textLangList[5].first.setPosition(1300, 820);

	AllSetActive(false);

}

void GameResult::Release()
{
}

void GameResult::Reset()
{
	AllSetActive(false);
}

void GameResult::Update(float dt)
{

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space) && GetActive())
	{
		SceneGame* scene = (SceneGame*)SCENE_MGR.GetCurrScene();
		scene->SetIsReStart(true);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Escape) && GetActive())
	{
		SCENE_MGR.GetCurrScene()->Exit();
		FRAMEWORK.GetWindow().close();
	}
}

void GameResult::Draw(sf::RenderWindow& window)
{
	window.draw(panel);
	window.draw(upLine);
	window.draw(downLine);
	window.draw(spaceButtonIcon);
	window.draw(exitButtonIcon);
	window.draw(playerDeadIcon, &shader);
	window.draw(leftIcon);
	window.draw(spaceIcon);
	window.draw(rightIcon);
	window.draw(qIcon);
	window.draw(eIcon);
	window.draw(rIcon);
	window.draw(mIcon);

	for (auto& it : textLangList)
	{
		window.draw(it.first);
	}

	window.draw(totalTimeValueText);
	window.draw(stageValueText);
}
void GameResult::SetPosition(const sf::Vector2f& p)
{
	position = p;
	panel.setPosition(p);

}

void GameResult::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
	panel.setPosition({ x,y });

}

void GameResult::SetOrigin(Origins origin)
{
	GameObject::SetOrigin(origin);
	if (this->origin != Origins::CUSTOM)
	{
		Utils::SetOrigin(panel, origin);
		Utils::SetOrigin(upLine, origin);
		Utils::SetOrigin(downLine, origin);
		Utils::SetOrigin(spaceButtonIcon, origin);
		Utils::SetOrigin(exitButtonIcon, origin);
		Utils::SetOrigin(playerDeadIcon, origin);
		Utils::SetOrigin(leftIcon, origin);
		Utils::SetOrigin(spaceIcon, origin);
		Utils::SetOrigin(rightIcon, origin);
		Utils::SetOrigin(qIcon, origin);
		Utils::SetOrigin(eIcon, origin);
		Utils::SetOrigin(rIcon, origin);
		Utils::SetOrigin(mIcon, origin);
	}

}

void GameResult::SetOrigin(float x, float y)
{
	GameObject::SetOrigin(x, y);
	panel.setOrigin(x, y);
	upLine.setOrigin(x, y);
	downLine.setOrigin(x, y);
	spaceButtonIcon.setOrigin(x, y);
	exitButtonIcon.setOrigin(x, y);
	playerDeadIcon.setOrigin(x, y);
	leftIcon.setOrigin(x, y);
	spaceIcon.setOrigin(x, y);
	rightIcon.setOrigin(x, y);
	qIcon.setOrigin(x, y);
	eIcon.setOrigin(x, y);
	rIcon.setOrigin(x, y);
	mIcon.setOrigin(x, y);
}

void GameResult::SetOrigin(sf::RectangleShape& rect, Origins origin)
{
	GameObject::SetOrigin(origin);

}
void GameResult::SetSprite(sf::Sprite& sprite, const std::string& textureId, sf::Vector2f size, sf::Vector2f pos)
{
	sprite.setTexture(*RESOURCE_MGR.GetTexture(textureId));
	sprite.setScale(size);
	sprite.setPosition(pos);
}

void GameResult::SetRectangleShape(sf::RectangleShape& rect, sf::Vector2f size, sf::Vector2f pos, sf::Color color)
{
	rect.setFillColor(color);
	rect.setSize(size);
	rect.setPosition(pos);
}
void GameResult::AllSetLangText(Languages lang)
{
	StringTable* stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);

	for (auto& pair : textLangList)
	{
		pair.first.setFont(*RESOURCE_MGR.GetFont("fonts/NanumSquareB.ttf"));
		pair.first.setCharacterSize(30);
		pair.first.setString(stringTable->GetUni(pair.second, lang));
		pair.first.setColor(sf::Color::White);
		Utils::SetOrigin(pair.first, Origins::ML);
	}
}

void GameResult::AllSetActive(bool isActive)
{

	SetActive(isActive);
}
