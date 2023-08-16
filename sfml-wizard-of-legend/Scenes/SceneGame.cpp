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
#include "Lancer.h"
#include "Archer.h"
#include "Player.h"
#include "ElementalSpell.h"
#include "Monster.h"
#include "TileInfoTable.h"
#include "rapidcsv.h"
#include "Tile.h"
#include "BoxCollider2D.h"
#include "DestructibleGo.h"
#include "Particle.h"

SceneGame::SceneGame() : Scene(SceneId::Game)
{
}

void SceneGame::Init()
{
	Release();
	auto size = FRAMEWORK.GetWindowSize();

	player = (Player*)AddGo(new Player());
	player->SetPosition(0, 0);
	player->sprite.setScale(4, 4);
	player->SetOrigin(Origins::MC);
	player->sortLayer = 5;
	player->SetScene(this);

	LoadFromCSV("tables/TileInfoTable.csv");

	tempWindSlash = (ElementalSpell*)AddGo(new ElementalSpell());
	tempWindSlash->SetScene(this);
	tempWindSlash->SetPlayer(player);
	tempWindSlash->sortLayer = 21;


	Monster* go = CreateMonster(MonsterId::Archer);
	monster = go;
	monster->SetPlayer(player);
	monster->SetTiles(&tilesWorld);
	player->SetMonster(go);
	tempWindSlash->SetMonster(monster);

	// TEST Particle
	CreateParticle(1000);

	// TEST DestructibleObject
	DestructibleGo* deco = (DestructibleGo*)AddGo(new DestructibleGo("graphics/editor/FireTileSet.png", "Deco"));
	deco->sortLayer = 2;
	deco->SetPosition(64.0f, 64.0f);
	deco->SetScale(4.0f, 4.0f);
	deco->SetTextureRect(sf::IntRect(304, 0, 32, 80));
	deco->SetParticlePool(&particlePool);

	for (auto go : gameObjects)
	{
		if (go->GetName() == "a")
		{
			std::cout << "";
		}
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

	ClearObjectPool(particlePool);
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);	
	debugTimer += dt;
	worldView.setCenter(player->GetPosition());
	//isCol = colliderManager.ObbCol(monster->rect, tempWindSlash->GetCollider());
	//isCol = colliderManager.ObbCol(tempWindSlash->GetCollider(), monster->rect);

	//if (debugTimer > debugDuration && !isCol)
	//{
	//	debugTimer = 0.f;
	//	std::cout << "OBB is Failed" << std::endl;
	//}
	//if (isCol)
	//{
	//	std::cout << "OBB is Succesd" << std::endl;
	//}


	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tilde))
	{
		SCENE_MGR.ChangeScene(SceneId::Editor);
	}

	// TEST Particle
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Q))
	{
		for (int i = 0; i < 50; i++)
		{
			DestructibleGo* deco = (DestructibleGo*)FindGo("Deco");
			deco->SetParticle({ 0.0f, 0.0f });
		}
	}
	//std::cout << "particle test : " << particlePool.GetUseList().size() << std::endl;
}

template<typename T>
inline void SceneGame::ClearObjectPool(ObjectPool<T>& pool)
{
	for (auto obj : pool.GetUseList())
	{
		RemoveGo(obj);
	}
	pool.Clear();
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

	int maxTileIndexX = 0;
	int maxTileIndexY = 0;

	for (size_t i = 0; i < doc.GetRowCount(); i++)
	{
		int tileIndexX = doc.GetCell<int>("tileIndexX", i);
		int tileIndexY = doc.GetCell<int>("tileIndexY", i);

		if (tileIndexX > maxTileIndexX) maxTileIndexX = tileIndexX;
		if (tileIndexY > maxTileIndexY) maxTileIndexY = tileIndexY;
	}
	rows = maxTileIndexX + 1;
	cols = maxTileIndexY + 1;
	CreateTile2dVector(rows, cols);

	for (size_t i = 0; i < doc.GetRowCount(); i++)
	{
		std::string tileName = doc.GetCell<std::string>("tileName", i);

		int tileIndexX = doc.GetCell<int>("tileIndexX", i);
		int tileIndexY = doc.GetCell<int>("tileIndexY", i);
		int tileType   = doc.GetCell<int>("tileType", i);
		int tileSize   = doc.GetCell<int>("tileSize", i);
		int tileScale  = doc.GetCell<int>("tileScale", i);
		int tileLayer  = doc.GetCell<int>("tileLayer", i);

		std::string textureId = doc.GetCell<std::string>("textureId", i);
		sf::IntRect topTextureRect(doc.GetCell<int>("topTextureRectL", i), doc.GetCell<int>("topTextureRectT", i), tileSize, tileSize);
		sf::IntRect bottomTextureRect(doc.GetCell<int>("bottomTextureRectL", i), doc.GetCell<int>("bottomTextureRectT", i), tileSize, tileSize);

		Tile* tile = (Tile*)FindGo(tileName);
		tile->SetIndex(tileIndexX, tileIndexY);
		tile->SetType(static_cast<Tile::TileType>(tileType));
		tile->SetTileSize(tileSize);
		tile->SetScale(tileScale);
		tile->SetLayer(tileLayer);
		tile->SetTexture(textureId);
		tile->SetTextureRectTop(topTextureRect, textureId);
		tile->SetTextureRectBottom(bottomTextureRect, textureId);
		tile->SetOrigin(Origins::MC);
		tilesWorld[tileIndexX][tileIndexY] = tile;
	}
	//std::cout << "SYSTEM : Load Success" << std::endl;
}

Monster* SceneGame::CreateMonster(MonsterId id)
{
	Monster* monster = nullptr;
	switch (id)
	{
	case MonsterId::Ghoul:
		monster = dynamic_cast<Monster*>(AddGo(new Monster(id)));
		break;
	case MonsterId::GhoulLarge:
		monster = dynamic_cast<Monster*>(AddGo(new Monster(id)));
		break;
	case MonsterId::Lancer:
		monster = dynamic_cast<Monster*>(AddGo(new Lancer(id)));
		break;
	case MonsterId::Archer:
		monster = dynamic_cast<Monster*>(AddGo(new Archer(id)));
		break;
	}
	return monster;
}

void SceneGame::CreateTile2dVector(int rows, int cols)
{
	tilesWorld.resize(rows, std::vector<Tile*>(cols, nullptr));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Tile* tile = CreateTile("Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")", i * tileSize, j * tileSize);
			tilesWorld[i][j] = tile;
		}
	}
}

void SceneGame::CreateParticle(int count)
{
	particlePool.OnCreate = [this](Particle* particle)
	{
		int rand = 16 * Utils::RandomRange(0, 8);

		particle->SetName("Particle");
		particle->SetDuration(1.0f);
		particle->SetTexture("graphics/game/Fire.png");
		particle->SetTextureRect(sf::IntRect(rand, 0, 16, 16));
		particle->sortLayer = 25;
		particle->sortOrder = 0;
		particle->SetPool(&particlePool);
		particle->Reset();
		particle->SetActive(false);
	};
	particlePool.Init(count);
}