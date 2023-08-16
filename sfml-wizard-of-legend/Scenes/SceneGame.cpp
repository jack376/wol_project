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
#include "BreakableObj.h"
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
	player->sortLayer = 20;
	player->SetScene(this);

	LoadFromCSV("tables/TileInfoTable.csv");

	tempWindSlash = (ElementalSpell*)AddGo(new ElementalSpell());
	tempWindSlash->SetScene(this);
	tempWindSlash->SetPlayer(player);
	tempWindSlash->sortLayer = 21;

	Monster* go = (Monster*)AddGo(new Monster(MonsterId::Ghoul));

	player->SetMonster(go);

	// Create Particle
	CreateParticle(1000);

	SpawnBreakableObj("graphics/editor/FireTileSet.png", 15);

	TestMultipleRandomPaths();

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

	ClearObjectPool(particlePool);
}

void SceneGame::Exit()
{
	Scene::Exit();
}

void SceneGame::Update(float dt)
{
	Scene::Update(dt);

	worldView.setCenter(player->GetPosition());

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tilde))
	{
		SCENE_MGR.ChangeScene(SceneId::Editor);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Q))
	{
		BreakableObj* targetObj = (BreakableObj*)FindGo("Object" + count);
		targetObj->OnHitBreakableObj(targetObj->GetName(), 2);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::E))
	{
		SpawnBreakableObj("graphics/editor/FireTileSet.png", 15);
		count = 0;
	}

	// TEST Particle
	/*
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::G))
	{
		BreakableObj* deco = (BreakableObj*)FindGo("Deco");
		deco->SetParticle({ player->GetPosition() }, 50);
	}
	*/
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
		tile->SetOrigin(Origins::TL);
		tilesWorld[tileIndexX][tileIndexY] = tile;
	}
	std::cout << "SYSTEM : Load Success" << std::endl;
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

void SceneGame::SpawnBreakableObj(const std::string& id, int count)
{
	std::vector<sf::Vector2f> objectSpawnArea;

	for (size_t row = 0; row < tilesWorld.size(); row++)
	{
		for (size_t col = 0; col < tilesWorld[row].size(); col++)
		{
			if (tilesWorld[row][col]->GetType() == Tile::TileType::EventTrigger)
			{
				objectSpawnArea.push_back(tilesWorld[row][col]->GetPosition());
			}
		}
	}

	if (objectSpawnArea.empty())
	{ 
		return; 
	}

	for (int i = 0; i < count && !objectSpawnArea.empty(); i++)
	{
		int randomIndex = Utils::RandomRange(0, objectSpawnArea.size() - 1);
		int randomNumber = Utils::RandomRange(0, 6);
		sf::Vector2f randomPosition = objectSpawnArea[randomIndex];

		objectSpawnArea.erase(objectSpawnArea.begin() + randomIndex);

		BreakableObj* object = (BreakableObj*)AddGo(new BreakableObj(id, "Object" + i));
		object->sortLayer = 30;
		object->SetActive(true);
		object->SetPlayer(player);
		object->SetPosition(randomPosition);
		object->SetScale(4.0f, 4.0f);
		object->SetTextureRect(sf::IntRect(368 + (randomNumber * 16), 384, 16, 32));
		object->SetParticlePool(&particlePool);
		object->Reset();
	}
}

void SceneGame::DestroyBreakableObj(BreakableObj* obj)
{
	RemoveGo(obj);
	count++;
}

std::vector<sf::Vector2i> SceneGame::CreateRandomPath(Tile::TileType startType, Tile::TileType traversableType, int pathLength)
{
	std::vector<sf::Vector2i> path;
	std::vector<sf::Vector2i> possibleStarts;

	for (int y = 0; y < tilesWorld.size(); y++) 
	{
		for (int x = 0; x < tilesWorld[y].size(); x++) 
		{
			if (tilesWorld[y][x]->GetType() == startType) 
			{
				possibleStarts.push_back(sf::Vector2i(x, y));
			}
		}
	}

	if (possibleStarts.empty())
	{
		return path;
	}

	sf::Vector2i currentPoint = possibleStarts[Utils::RandomRange(0, possibleStarts.size() - 1)];
	path.push_back(currentPoint);

	for (int i = 0; i < pathLength - 1; i++) 
	{
		std::vector<sf::Vector2i> possibleMoves = 
		{
			{  0,  1  }, // Up
			{  1,  0  }, // Right
			{  0, -1  }, // Down
			{ -1,  0  }  // Left
		};

		possibleMoves.erase(std::remove_if(possibleMoves.begin(), possibleMoves.end(),[&](const sf::Vector2i& move) 
			{ 
				sf::Vector2i nextPoint = currentPoint + move;
				return 
					nextPoint.x < 0 || 
					nextPoint.x >= tilesWorld[0].size() || 
					nextPoint.y < 0 || 
					nextPoint.y >= tilesWorld.size() || 
					tilesWorld[nextPoint.y][nextPoint.x]->GetType() != traversableType;
			}
		), possibleMoves.end());

		if (!possibleMoves.empty()) 
		{
			sf::Vector2i chosenMove = possibleMoves[Utils::RandomRange(0, possibleMoves.size() - 1)];
			currentPoint += chosenMove;
			path.push_back(currentPoint);
		}
		else 
		{
			break;
		}
	}
	return path;
}

void SceneGame::TestRandomPath()
{
	std::vector<sf::Vector2i> path = CreateRandomPath(Tile::TileType::Ground, Tile::TileType::Ground, 256);

	for (const auto& point : path) 
	{
		tilesWorld[point.y][point.x]->SetType(Tile::TileType::EventTrigger);
	}
}

std::vector<std::vector<sf::Vector2i>> SceneGame::CreateMultipleRandomPaths(Tile::TileType startType, Tile::TileType traversableType, int mainPathLength, int branchCount, int branchLength)
{
	std::vector<std::vector<sf::Vector2i>> allPaths;

	std::vector<sf::Vector2i> mainPath = CreateRandomPath(startType, traversableType, mainPathLength);
	allPaths.push_back(mainPath);

	int branchingPointIndex = Utils::RandomRange(1, mainPathLength - 2);
	sf::Vector2i branchingPoint = mainPath[branchingPointIndex];

	for (int i = 0; i < branchCount; i++)
	{
		tilesWorld[branchingPoint.y][branchingPoint.x]->SetType(startType);
		std::vector<sf::Vector2i> branchPath = CreateRandomPath(startType, traversableType, branchLength);
		allPaths.push_back(branchPath);

		tilesWorld[branchingPoint.y][branchingPoint.x]->SetType(traversableType);
	}

	return allPaths;
}

void SceneGame::TestMultipleRandomPaths()
{
	auto allPaths = CreateMultipleRandomPaths(Tile::TileType::Ground, Tile::TileType::Ground, 64, 3, 20);

	for (const auto& path : allPaths)
	{
		for (const auto& point : path)
		{
			tilesWorld[point.y][point.x]->SetType(Tile::TileType::EventTrigger);
		}
	}
}