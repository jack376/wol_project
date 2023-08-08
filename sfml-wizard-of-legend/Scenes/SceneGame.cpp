#include "stdafx.h"
#include "SceneGame.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "Framework.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "Monster.h"
#include "Player.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
}

void SceneGame::Init()
{
	Release();
	auto size = FRAMEWORK.GetWindowSize();

	player = (Player*)AddGo(new Player());
	player->SetPosition(0, 0);
	player->sprite.setScale(4.5, 4.5);
	player->SetOrigin(Origins::MC);
	player->sortLayer = 20;
	player->SetScene(this);

	//GameObject* go = AddGo(new Monster(MonsterId::Ghoul));
	//SetInitValue(go, Origins::MC, size * 0.5f);

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneGame::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;  
	}
}

void SceneGame::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(0, 0);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);	
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

void SceneGame::SetInitValue(GameObject* go, Origins origin, sf::Vector2f pos, float angle, int layer)
{
	go->SetOrigin(origin);
	go->SetPosition(pos);
	//go->SetRotation(angle);
	go->sortLayer = layer;
}
