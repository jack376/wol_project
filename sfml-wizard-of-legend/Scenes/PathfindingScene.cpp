#include "stdafx.h"
#include "PathfindingScene.h"

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
#include "Mage.h"
#include "Player.h"
#include "ElementalSpell.h"
#include "Monster.h"
#include "TileInfoTable.h"
#include "rapidcsv.h"
#include "Tile.h"
#include "BoxCollider2D.h"
#include "DecoGo.h"
#include "Particle.h"
#include "AS.h"


PathfindingScene::PathfindingScene()
{
}

void PathfindingScene::Init()
{
	Release();

	LoadFromCSV("tables/TileInfoTable.csv");
	TilesToIntMap();

	for (auto go : gameObjects)
	{
		if (go->GetName() == "a")
		{
			std::cout << "";
		}
		go->Init();
	}
}

void PathfindingScene::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void PathfindingScene::Enter()
{
	auto size = FRAMEWORK.GetWindowSize();
	worldView.setSize(size);
	worldView.setCenter(0, 0);

	uiView.setSize(size);
	uiView.setCenter(size * 0.5f);

	Scene::Enter();
}

void PathfindingScene::Exit()
{
	Scene::Exit();
}

void PathfindingScene::Update(float dt)
{
	Scene::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);


	// Camera
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

	// Zoom
	Scene* scene = SCENE_MGR.GetCurrScene();
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Z) && INPUT_MGR.GetKey(sf::Keyboard::LControl) == false) { scene->Zoom(zoomInFactor); }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::X) && INPUT_MGR.GetKey(sf::Keyboard::LControl) == false) { scene->Zoom(zoomOutFactor); }


	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tilde))
	{
		SCENE_MGR.ChangeScene(SceneId::Editor);
	}

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Space))
	{
		TilesToIntMap();
		path = _AS.aStarSearch(intMap, AS::Pair(2, 28), AS::Pair(24, 17));
	}
	tilesWorld[2][28]->SetShapeColor(sf::Color::Blue);
	tilesWorld[24][17]->SetShapeColor(sf::Color::Magenta);

	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Enter))
	{
		if (!path.second.empty())
		{
			path.second.pop();
		}
	}
	if (!path.second.empty())
	{
		tilesWorld[path.second.top().first][path.second.top().second]->SetShapeColor(sf::Color::Black);
	}

}

void PathfindingScene::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

Tile* PathfindingScene::CreateTile(const std::string& name, float posX, float posY, int sort)
{
	Tile* tile = (Tile*)AddWithoutCheckGo(new Tile(name));
	tile->sortLayer = sort;
	tile->SetPosition(posX, posY);

	return tile;
}

void PathfindingScene::LoadFromCSV(const std::string& path)
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
		int tileType = doc.GetCell<int>("tileType", i);
		int tileSize = doc.GetCell<int>("tileSize", i);
		int tileScale = doc.GetCell<int>("tileScale", i);
		int tileLayer = doc.GetCell<int>("tileLayer", i);

		std::string textureId = doc.GetCell<std::string>("textureId", i);
		sf::IntRect topTextureRect(doc.GetCell<int>("topTextureRectL", i), doc.GetCell<int>("topTextureRectT", i), tileSize, tileSize);
		sf::IntRect bottomTextureRect(doc.GetCell<int>("bottomTextureRectL", i), doc.GetCell<int>("bottomTextureRectT", i), tileSize, tileSize);

		Tile* tile = (Tile*)FindGo(tileName);
		tile->SetIndex(tileIndexX, tileIndexY);
		tile->SetType(static_cast<TileType>(tileType));
		tile->SetTileSize(tileSize);
		tile->SetScale(tileScale);
		tile->SetLayer(tileLayer);
		tile->SetTexture(textureId);
		tile->SetTextureRectTop(topTextureRect, textureId);
		tile->SetTextureRectBottom(bottomTextureRect, textureId);
		tile->SetOrigin(Origins::TL);
		tilesWorld[tileIndexX][tileIndexY] = tile;
	}
	//std::cout << "SYSTEM : Load Success" << std::endl;
}

void PathfindingScene::CreateTile2dVector(int rows, int cols)
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

void PathfindingScene::TilesToIntMap()
{
	intMap.clear();
	for (auto& tiles : tilesWorld)
	{
		std::vector<int> row;
		for (auto& tile : tiles)
		{
			if (tile->GetType() == TileType::Ground)
				row.push_back(0);
			else
				row.push_back(-1);
		}
		intMap.push_back(row);
	}
}
