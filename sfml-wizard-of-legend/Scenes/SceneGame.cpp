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
#include "ElementalSpell.h"
#include "Monster.h"
#include "TileInfoTable.h"
#include "rapidcsv.h"
#include "Tile.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
}

void SceneGame::Init()
{
	Release();
	auto size = FRAMEWORK.GetWindowSize();

	int rows = 32;
	int cols = 32;
	float tileSize = 64.0f;

	player = (Player*)AddGo(new Player());
	player->SetPosition(0, 0);
	player->sprite.setScale(4, 4);
	player->SetOrigin(Origins::MC);
	player->sortLayer = 20;
	player->SetScene(this);

	tilesWorld.resize(rows, std::vector<Tile*>(cols, nullptr));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Tile* tile = CreateTile("Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")", i * tileSize, j * tileSize);
			tilesWorld[i][j] = tile;
		}
	}
	LoadFromCSV("tables/TileInfoTable.csv");

	tempWindSlash = (ElementalSpell*)AddGo(new ElementalSpell());
	tempWindSlash->SetScene(this);
	tempWindSlash->SetPlayer(player);
	tempWindSlash->sortLayer = 21;

	Monster* go = (Monster*)AddGo(new Monster(MonsterId::Ghoul));

	player->SetMonster(go);

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

	// Test Code
	cameraDirection.x = INPUT_MGR.GetAxis(Axis::Horizontal);
	cameraDirection.y = INPUT_MGR.GetAxis(Axis::Vertical);

	float magnitude = Utils::Magnitude(cameraDirection);
	if (magnitude > 0.0f)
	{
		if (magnitude > 1.f)
		{
			cameraDirection /= magnitude;
		}
		cameraPosition += cameraDirection * cameraSpeed * dt;
		camera.setPosition(cameraPosition);
	}
	worldView.setCenter(cameraPosition);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tilde))
	{
		SCENE_MGR.ChangeScene(SceneId::Editor);
	}
}

void SceneGame::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

Tile* SceneGame::CreateTile(const std::string& name, float posX, float posY, int sort)
{
	Tile* tile = (Tile*)AddGo(new Tile(name));
	tile->sortLayer = sort;
	tile->SetPosition(posX, posY);

	return tile;
}

void SceneGame::LoadFromCSV(const std::string& path)
{
	rapidcsv::Document doc(path);

	for (size_t i = 0; i < doc.GetRowCount(); i++)
	{
		std::string tileName = doc.GetCell<std::string>("tileName", i);
		int tileIndexX = doc.GetCell<int>("tileIndexX", i);
		int tileIndexY = doc.GetCell<int>("tileIndexY", i);
		int tileType = doc.GetCell<int>("tileType", i);
		int tileSize = doc.GetCell<int>("tileSize", i);
		int tileScaleFactor = doc.GetCell<int>("tileScaleFactor", i);
		int tileLayer = doc.GetCell<int>("tileLayer", i);

		std::string textureId = doc.GetCell<std::string>("textureId", i);
		sf::IntRect textureRect
		(
			doc.GetCell<int>("textureRectLeft", i),
			doc.GetCell<int>("textureRectTop", i),
			doc.GetCell<int>("textureRectWidth", i),
			doc.GetCell<int>("textureRectHeight", i)
		);

		Tile* tile = (Tile*)FindGo(tileName);
		tile->SetIndex(tileIndexX, tileIndexY);
		tile->SetType(static_cast<Tile::TileType>(tileType));
		tile->SetTileSize(tileSize);
		tile->SetScale(tileScaleFactor);
		tile->SetLayer(tileLayer);
		tile->SetTexture(textureId);
		tile->SetTextureRect(textureRect, textureId);
		tile->SetOrigin(Origins::MC);
		tilesWorld[tileIndexX][tileIndexY] = tile;
	}
	std::cout << "SYSTEM : Load Success" << std::endl;
}
