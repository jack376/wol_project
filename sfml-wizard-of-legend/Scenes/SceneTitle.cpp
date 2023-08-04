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


SceneTitle::SceneTitle() : Scene(SceneId::Title)
{
	//resourceListPath = "scripts/SceneTitleResourceList.csv";
}

void SceneTitle::Init()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	StringTable* stringTable = DATATABLE_MGR.Get<StringTable>(DataTable::Ids::String);


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
}

void SceneTitle::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneTitle::ButtonInit(UIButton* btn, sf::Vector2f pos, sf::Vector2f size, SceneId id)
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();

	btn->SetPosition(pos);
	btn->sprite.setScale(size);
	btn->SetOrigin(Origins::MC);
	btn->sortLayer = 102;
	btn->text.setPosition(btn->text.getPosition().x, btn->text.getPosition().y - 8.f);

	btn->OnEnter = [this, btn]() {
	};
	btn->OnExit = [this, btn]() {
		btn->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
		btn->text.setFillColor(sf::Color::Color(255, 255, 255, 255));
		btn->text.setOutlineColor(sf::Color::Color(0, 0, 0, 255));

	};
	btn->OnClick = [this, btn, id]() {
		btn->sprite.setColor(sf::Color::Color(255, 255, 255, 255));
		btn->text.setFillColor(sf::Color::Color(255, 255, 255, 255));
		btn->text.setOutlineColor(sf::Color::Color(0, 0, 0, 255));
		//SCENE_MGR.ChangeScene(id);
	};	
	btn->OnClicking = [this, btn]() {
		btn->sprite.setColor(sf::Color::Color(255, 255, 255, 180));
		btn->text.setFillColor(sf::Color::Color(255, 255, 255, 180));
		btn->text.setOutlineColor(sf::Color::Color(0, 0, 0, 180));

	};	
}
