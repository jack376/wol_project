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
	
	// Tile
	int rows = 64;
	int cols = 64;
	tiles.resize(rows, std::vector<Tile*>(cols, nullptr));

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Tile* tile = CreateTile("Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")", i * tileSize, j * tileSize);
			tiles[i][j] = tile;
			AddGo(tile);
			std::cout << "Loading : Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")" << std::endl;
		}
	}

	// Button
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	resolutionScaleFactor = windowSize.x / 1920.0f;

	BaseUI* zoomInButton = (BaseUI*)AddGo(new BaseUI("ZoomInButton", UiType::Text));
	zoomInButton->sortLayer = 100;
	zoomInButton->text.setString(L"ZOOM+ (Z)");
	zoomInButton->SetCharacterSize(32);
	zoomInButton->SetPosition(50.0f, 50.0f * resolutionScaleFactor);
	zoomInButton->SetColor(sf::Color::Black);
	zoomInButton->SetTextColor(sf::Color::White);
	zoomInButton->SetStrokeColor(sf::Color::White);
	zoomInButton->OnEnter = [zoomInButton, this]()
	{
		isUiButtonActive = true;
		zoomInButton->SetStrokeColor(sf::Color::Cyan);
	};
	zoomInButton->OnExit = [zoomInButton, this]()
	{
		isUiButtonActive = false;
		zoomInButton->SetStrokeColor(sf::Color::White);
	};
	zoomInButton->OnClick = [zoomInButton, this]()
	{
		isUiButtonActive = true;
		Scene* scene = SCENE_MGR.GetCurrScene();
		scene->Zoom(zoomInFactor);
		zoomInButton->SetStrokeColor(sf::Color::Green);
	};

	BaseUI* zoomOutButton = (BaseUI*)AddGo(new BaseUI("ZoomOutButton", UiType::Text));
	zoomOutButton->sortLayer = 100;
	zoomOutButton->text.setString(L"ZOOM- (X)");
	zoomOutButton->SetCharacterSize(32);
	zoomOutButton->SetPosition(50.0f, 150.0f * resolutionScaleFactor);
	zoomOutButton->SetColor(sf::Color::Black);
	zoomOutButton->SetTextColor(sf::Color::White);
	zoomOutButton->SetStrokeColor(sf::Color::White);
	zoomOutButton->OnEnter = [zoomOutButton, this]()
	{
		isUiButtonActive = true;
		zoomOutButton->SetStrokeColor(sf::Color::Cyan);
	};
	zoomOutButton->OnExit = [zoomOutButton, this]()
	{
		isUiButtonActive = false;
		zoomOutButton->SetStrokeColor(sf::Color::White);
	};
	zoomOutButton->OnClick = [zoomOutButton, this]()
	{
		isUiButtonActive = true;
		Scene* scene = SCENE_MGR.GetCurrScene();
		scene->Zoom(zoomOutFactor);
		zoomOutButton->SetStrokeColor(sf::Color::Green);
	};

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

	// Zoom
	Scene* scene = SCENE_MGR.GetCurrScene();
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Z))
	{
		scene->Zoom(zoomInFactor);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::X))
	{
		scene->Zoom(zoomOutFactor);
	}

	// Select, Deselect
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::C))
	{
		std::vector<Tile*> selectedTiles = GetSelectedTiles();
		SetSelectedTilesState(selectedTiles, Tile::TileState::Copy);
	}
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Right))
	{
		std::vector<Tile*> allTiles = GetAllTiles();
		SetSelectedTilesState(allTiles, Tile::TileState::None);
	}

	// Copy
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::M))
	{
		std::vector<Tile*> selectedTiles = GetSelectedTiles();
		SetSelectedTilesArea();
	}

	// Camera Move
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
	tile->SetStateColor(Tile::TileState::None);
	tile->OnEnter = [tile]()
	{
		if (tile->GetState() == Tile::TileState::None)
		{
			tile->SetShapeColor(sf::Color::Red);
		}
	};
	tile->OnExit = [tile]()
	{
		if (tile->GetState() == Tile::TileState::None)
		{
			tile->SetStateColor(Tile::TileState::None);
		}
	};
	tile->OnClickDown = [tile, this]()
	{
		if (!isUiButtonActive)
		{
			startTileIndex = GetCurrentTileIntIndex();
			isMouseSelect = true;
		}
	};
	tile->OnClickDrag = [tile, this]()
	{
		if (!isUiButtonActive && isMouseSelect)
		{
			endTileIndex = GetCurrentTileIntIndex();
			SetSelectedTilesArea();
		}
	};
	tile->OnClickUp = [tile, this]()
	{
		if (!isUiButtonActive && isMouseSelect)
		{
			endTileIndex = GetCurrentTileIntIndex();
			isMouseSelect = false;
		}
	};
	return tile;
}

std::vector<Tile*> SceneEditor::GetAllTiles()
{
	std::vector<Tile*> allTiles;
	for (auto& row : tiles)
	{
		for (Tile* tile : row)
		{
			allTiles.push_back(tile);
		}
	}
	return allTiles;
}

std::vector<Tile*> SceneEditor::GetSelectedTiles()
{
	std::vector<Tile*> selectedTiles;
	for (auto& row : tiles) 
	{
		for (Tile* tile : row) 
		{
			if (tile->GetState() == Tile::TileState::Select)
			{
				selectedTiles.push_back(tile);
			}
		}
	}
	return selectedTiles;
}

void SceneEditor::SetSelectedTilesState(std::vector<Tile*>& selectedTiles, Tile::TileState state)
{
	for (Tile* tile : selectedTiles)
	{
		tile->SetState(state);
		tile->SetStateColor(state);
	}
}

void SceneEditor::SetSelectedTilesArea()
{
	for (int x = std::min(startTileIndex.x, endTileIndex.x); x <= std::max(startTileIndex.x, endTileIndex.x); ++x)
	{
		for (int y = std::min(startTileIndex.y, endTileIndex.y); y <= std::max(startTileIndex.y, endTileIndex.y); ++y)
		{
			tiles[x][y]->SetState(Tile::TileState::Select);
			tiles[x][y]->SetStateColor(Tile::TileState::Select);
		}
	}
}

sf::Vector2i SceneEditor::GetCurrentTileIntIndex()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

	int xIndex = static_cast<int>(worldMousePos.x) / tileSize;
	int yIndex = static_cast<int>(worldMousePos.y) / tileSize;

	return sf::Vector2i(xIndex, yIndex);
}