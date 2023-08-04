#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SceneEditor.h"
#include "SpriteGo.h"
#include "TextGo.h"
#include "BaseUI.h"
#include "Tile.h"

SceneEditor::SceneEditor() : Scene(SceneId::Editor)
{
	resourceListPath = "scripts/SceneEditorResourceList.csv";
}

void SceneEditor::Init()
{
	Release();
	
	int rows = 50;
	int cols = 50;

	float tileSize = 64.0f;

	for (int i = 0; i < rows; i++) 
	{
		for (int j = 0; j < cols; j++) 
		{
			Tile* tile = CreateTile("Tile" + std::to_string(i * cols + j), i * 64.0f, j * 64.0f);
			tiles.push_back(tile);
			AddGo(tile);
		}
	}

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneEditor::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
	tiles.clear();
}

void SceneEditor::Enter()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	worldView.setSize(windowSize);
	worldView.setCenter({ 0, 0 });

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	cameraPosition = sf::Vector2f(windowSize * 0.5f);

	Scene::Enter();
}

void SceneEditor::Exit()
{
	Scene::Exit();
}

void SceneEditor::Update(float dt)
{
	Scene::Update(dt);

	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

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
}

void SceneEditor::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

Tile* SceneEditor::CreateTile(const std::string& name, float posX, float posY)
{
	Tile* tile = (Tile*)AddGo(new Tile(name));
	tile->sortLayer = 0;
	tile->SetShapePosition(posX, posY);
	tile->SetShapeColor(sf::Color(0, 192, 0, 255));
	tile->OnEnter = [tile]()
	{
		tile->SetShapeColor(sf::Color(0, 192, 0, 255));
	};
	tile->OnExit = [tile]()
	{
		tile->SetShapeColor(sf::Color(192, 0, 0, 255));
	};
	tile->OnClick = [tile]()
	{
		tile->SetShapeColor(sf::Color(0, 0, 192, 255));
	};

	return tile;
}
