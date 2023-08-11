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
#include "rapidcsv.h"

SceneEditor::SceneEditor() : Scene(SceneId::Editor)
{
}

void SceneEditor::Init()
{
	Release();

	windowSize = FRAMEWORK.GetWindowSize();
	resolutionScaleFactor = windowSize.x / fhdWidth;

	// Tile
	tilesWorld.resize(rows, std::vector<Tile*>(cols, nullptr));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Tile* tile = CreateTile("Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")", i * tileSize, j * tileSize);
			tilesWorld[i][j] = tile;
			//std::cout << "Loading : Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")" << std::endl;
		}
	}

	// Texture Atlas
	SpriteGo* atlasPreview = (SpriteGo*)AddGo(new SpriteGo("graphics/editor/FireTileSet.png", "AtlasPreview"));
	atlasPreview->sortLayer = 105;
	atlasPreview->SetOrigin(Origins::TL);
	atlasPreview->SetPosition(blankPos, blankPos);

	// TextureRect
	const int tilesPerRow = atlasTextureSize / tileTextureSize;
	const int uiSort = 110;
	for (int y = 0; y < tilesPerRow; ++y)
	{
		std::vector<sf::IntRect> colTiles;
		for (int x = 0; x < tilesPerRow; ++x)
		{
			sf::IntRect tileRect(y * tileTextureSize, x * tileTextureSize, tileTextureSize, tileTextureSize);
			colTiles.push_back(tileRect);
		}
		tileTextureAtlas.push_back(colTiles);
	}

	tilesPreview.resize(tilesPerRow, std::vector<Tile*>(tilesPerRow, nullptr));
	for (int i = 0; i < tilesPerRow; i++)
	{
		for (int j = 0; j < tilesPerRow; j++)
		{
			Tile* tile = CreateTilePreview("TilePreview(" + std::to_string(i) + ", " + std::to_string(j) + ")", i * tileTextureSize, j * tileTextureSize, uiSort);
			tilesPreview[i][j] = tile;
		}
	}

	// Load CSV
	LoadFromCSV("tables/TileInfoTable.csv");

	// UI backgorund
	BaseUI* uiBackground = (BaseUI*)AddGo(new BaseUI("UiBackGround", UiType::Box));
	uiBackground->sortLayer = 100;
	uiBackground->SetSizeAdd(atlasTextureSize + blankPos * 2.0f, windowSize.y);
	uiBackground->SetColor(sf::Color(16, 16, 16, 255));
	uiBackground->SetStrokeColor(sf::Color(32, 32, 64, 255));

	// UI Palette Button
	/*
	std::map<int, std::function<void()>> paletteIndex =
	{
		{ 1,  [this]() { selectPaletteIndex = 1; }   },
		{ 2,  [this]() { selectPaletteIndex = 5; }   },
		{ 3,  [this]() { selectPaletteIndex = 12; }  },
		{ 4,  [this]() { selectPaletteIndex = 33; }  },
		{ 5,  [this]() { selectPaletteIndex = 37; }  },
		{ 6,  [this]() { selectPaletteIndex = 44; }  },
		{ 7,  [this]() { selectPaletteIndex = 69; }  },
		{ 8,  [this]() { selectPaletteIndex = 101; } },
		{ 9,  [this]() { selectPaletteIndex = 136; } },
		{ 10, [this]() { selectPaletteIndex = 353; } },
		{ 11, [this]() { selectPaletteIndex = 355; } },
		{ 12, [this]() { selectPaletteIndex = 364; } },
	};

	float buttonSize = 32.0f;
	float buttonPosX = blankPos;
	float buttonPosY = atlasTextureSize + blankPos * 2;
	float buttonsGap = buttonSize + buttonSize * 0.25f;
	int buttonTexIdx = 0;
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 0, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 1, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 2, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 3, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 4, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 5, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 6, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	CreateButton("typeSelectButton", "None", buttonPosX + buttonsGap * 7, buttonPosY, buttonSize, buttonTexIdx, [this]() { std::cout << "Button On" << std::endl; });
	*/

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
	tilesWorld.clear();
}

void SceneEditor::Enter()
{
	sf::Vector2f windowSize = FRAMEWORK.GetWindowSize();
	worldView.setSize(windowSize);
	worldView.setCenter({ 0, 0 });
	worldView.zoom(zoomDefaultFactor);

	uiView.setSize(windowSize);
	uiView.setCenter(windowSize * 0.5f);

	cameraPosition = sf::Vector2f(windowSize.x * 0.05f, windowSize.y * 0.2f);

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
	if (!INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKeyDown(sf::Keyboard::Z))
	{
		scene->Zoom(zoomInFactor);
	}
	if (!INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKeyDown(sf::Keyboard::X))
	{
		scene->Zoom(zoomOutFactor);
	}

	// Copy(Unused)
	if (INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKeyDown(sf::Keyboard::C))
	{
		std::vector<Tile*> selectedTiles = GetSelectedTiles();
		SetSelectedTilesState(Tile::TileState::Copy);
	}

	// Paste(Unused)
	if (INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKeyDown(sf::Keyboard::V))
	{
		std::vector<Tile*> selectedTiles = GetSelectedTiles();
		SetSelectedTilesArea();
	}

	// Deselect
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Right))
	{
		SetSelectedTilesState(Tile::TileState::Blank);
	}

	// Draw
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Q))
	{
		SetSelectedTilesDraw();
		SetSelectedTilesState(Tile::TileState::Blank);
	}

	// Save
	if (INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKeyDown(sf::Keyboard::S))
	{
		SaveToCSV("tables/TileInfoTable.csv");
	}

	// Undo
	if (INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKeyDown(sf::Keyboard::Z))
	{
		std::cout << "Undo Key" << std::endl;
	}

	// Redo
	if (INPUT_MGR.GetKey(sf::Keyboard::LControl) && INPUT_MGR.GetKey(sf::Keyboard::LShift) && INPUT_MGR.GetKeyDown(sf::Keyboard::Z))
	{
		std::cout << "Redo Key" << std::endl;
	}

	// SetTileType
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1))
	{
		SetSelectedTilesType(Tile::TileType::Ground);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2))
	{
		SetSelectedTilesType(Tile::TileType::Cliff);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3))
	{
		SetSelectedTilesType(Tile::TileType::Wall);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4))
	{
		SetSelectedTilesType(Tile::TileType::MonsterSpawn);
	}
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num5))
	{
		SetSelectedTilesType(Tile::TileType::EventTrigger);
	}
}

void SceneEditor::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

Tile* SceneEditor::CreateTile(const std::string& name, float posX, float posY, int sort)
{
	Tile* tile = (Tile*)AddGo(new Tile(name));
	tile->sortLayer = sort;
	tile->SetShapePosition(posX, posY);
	tile->SetSpritePosition(posX, posY);
	tile->SetState();
	tile->SetStateColor(Tile::TileState::Blank);
	tile->SetStrokeColor(sf::Color(64, 64, 64, 96));
	tile->OnEnter = [tile, this]()
	{
		if (!isUiButtonActive && tile->GetState() == Tile::TileState::Blank)
		{
			tile->SetStateColor(Tile::TileState::Select);
		}
	};
	tile->OnExit = [tile, this]()
	{
		if (!isUiButtonActive && tile->GetState() == Tile::TileState::Blank)
		{
			tile->SetStateColor(tile->GetState());
			if (tile->IsTypeView())
			{
				tile->SetTypeColor(tile->GetType());
			}
		}
	};
	tile->OnClickDown = [tile, this]()
	{
		if (!isUiButtonActive && tile->GetState() == Tile::TileState::Blank)
		{
			startTileIndex = GetCurrentTileIntIndex();
			isUiMouseSelect = false;
			isWorldMouseSelect = true;
		}
	};
	tile->OnClickDrag = [tile, this]()
	{
		if (!isUiButtonActive && isWorldMouseSelect && tile->GetState() == Tile::TileState::Blank)
		{
			endTileIndex = GetCurrentTileIntIndex();
			SetSelectedTilesArea();
		}
	};
	tile->OnClickUp = [tile, this]()
	{
		if (!isUiButtonActive && isWorldMouseSelect && tile->GetState() == Tile::TileState::Blank)
		{
			endTileIndex = GetCurrentTileIntIndex();
			isUiMouseSelect = true;
			isWorldMouseSelect = false;
		}
	};
	return tile;
}

std::vector<Tile*> SceneEditor::GetAllTiles()
{
	for (auto& row : tilesWorld)
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
	for (auto& row : tilesWorld) 
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

void SceneEditor::SetSelectedTilesState(Tile::TileState state)
{
	for (Tile* tile : selectedTiles)
	{
		tile->SetState(state);
		tile->SetStateColor(state);
	}
	selectedTiles.clear();
}

void SceneEditor::SetSelectedTilesType(Tile::TileType type)
{
	for (Tile* tile : selectedTiles)
	{
		tile->SetType(type);
		tile->SetTypeColor(type);
	}
	SetSelectedTilesState();
}

void SceneEditor::SetSelectedTilesDraw()
{
	if (selectedTiles.empty() || selectedPreview.empty())
	{
		return;
	}

	sf::Vector2i indexPreviewStart = selectedPreview[0]->GetIndex();

	int previewWidth  = std::abs(endPreviewIndex.x - startPreviewIndex.x) + 1;
	int previewHeight = std::abs(endPreviewIndex.y - startPreviewIndex.y) + 1;

	sf::Vector2i indexTilesStart = selectedTiles[0]->GetIndex();

	for (Tile* worldTile : selectedTiles)
	{
		sf::Vector2i indexWorld = worldTile->GetIndex();

		int offsetX = (indexWorld.x - indexTilesStart.x) % previewWidth;
		int offsetY = (indexWorld.y - indexTilesStart.y) % previewHeight;

		sf::Vector2i matchPreviewIndex(indexPreviewStart.x + offsetX, indexPreviewStart.y + offsetY);

		Tile* previewTile = tilesPreview[matchPreviewIndex.x][matchPreviewIndex.y];

		sf::IntRect previewRect = sf::IntRect(previewTile->GetIndex().x * tileSize, previewTile->GetIndex().y * tileSize, tileSize, tileSize);
		worldTile->SetTextureRect(previewRect, textureId);
	}
}

void SceneEditor::SetSelectedTilesArea()
{
	for (Tile* tile : selectedTiles)
	{
		tile->SetState(Tile::TileState::Blank);
		tile->SetStateColor(Tile::TileState::Blank);
		if (tile->IsTypeView())
		{
			tile->SetTypeColor(tile->GetType());
		}
	}
	selectedTiles.clear();

	for (int x = std::min(startTileIndex.x, endTileIndex.x); x <= std::max(startTileIndex.x, endTileIndex.x); ++x)
	{
		for (int y = std::min(startTileIndex.y, endTileIndex.y); y <= std::max(startTileIndex.y, endTileIndex.y); ++y)
		{
			tilesWorld[x][y]->SetState(Tile::TileState::Select);
			tilesWorld[x][y]->SetStateColor(Tile::TileState::Select);
			tilesWorld[x][y]->SetIndex(x, y);
			selectedTiles.push_back(tilesWorld[x][y]);

			std::cout << "TileType : " << (int)tilesWorld[x][y]->GetType() << std::endl;
		}
	}
}

sf::Vector2i SceneEditor::GetCurrentTileIntIndex()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

	int xIndex = static_cast<int>(worldMousePos.x) / tileSize;
	int yIndex = static_cast<int>(worldMousePos.y) / tileSize;

	int maxRangeIndex = atlasTextureSize / tileTextureSize;
	if (xIndex < 0 || xIndex >= maxRangeIndex || yIndex < 0 || yIndex >= maxRangeIndex)
	{
		xIndex = std::max(0, std::min(xIndex, maxRangeIndex - 1));
		yIndex = std::max(0, std::min(yIndex, maxRangeIndex - 1));
	}
	return sf::Vector2i(xIndex, yIndex);
}

Tile* SceneEditor::CreateTilePreview(const std::string& name, float posX, float posY, int sort)
{
	Tile* tilePreview = (Tile*)AddGo(new Tile(name));
	tilePreview->sortLayer = sort;
	tilePreview->SetShapePosition(posX + blankPos, posY + blankPos);
	tilePreview->SetSpritePosition(posX + blankPos, posY + blankPos);
	tilePreview->SetState(Tile::TileState::UI);
	tilePreview->SetType(Tile::TileType::None);
	tilePreview->SetStateColor(Tile::TileState::UI);
	tilePreview->SetStrokeColor(sf::Color(64, 64, 64, 96));
	tilePreview->OnEnter = [tilePreview]()
	{
		if (tilePreview->GetState() == Tile::TileState::UI)
		{
			tilePreview->SetShapeColor(sf::Color(128, 0, 0, 128));
		}
	};
	tilePreview->OnExit = [tilePreview]()
	{
		if (tilePreview->GetState() == Tile::TileState::UI)
		{
			tilePreview->SetStateColor(Tile::TileState::UI);
		}
	};
	tilePreview->OnClickDown = [tilePreview, this]()
	{
		if (tilePreview->GetState() == Tile::TileState::UI)
		{
			startPreviewIndex = GetCurrentPreviewIntIndex();
			isUiMouseSelect = true;
			isWorldMouseSelect = false;
		}
	};
	tilePreview->OnClickDrag = [tilePreview, this]()
	{
		if (isUiMouseSelect && tilePreview->GetState() == Tile::TileState::UI)
		{
			endPreviewIndex = GetCurrentPreviewIntIndex();
			SetSelectedPreviewArea();
		}
	};
	tilePreview->OnClickUp = [tilePreview, this]()
	{
		if (isUiMouseSelect && tilePreview->GetState() == Tile::TileState::UI)
		{
			endPreviewIndex = GetCurrentPreviewIntIndex();
			isUiMouseSelect = false;
			isWorldMouseSelect = true;
		}
	};
	return tilePreview;
}

void SceneEditor::SetSelectedPreviewState(Tile::TileState state)
{
	for (Tile* tile : selectedPreview)
	{
		tile->SetState(state);
		tile->SetStateColor(state);
	}
	selectedPreview.clear();
}

void SceneEditor::SetSelectedPreviewArea()
{
	for (Tile* tile : selectedPreview)
	{
		tile->SetState(Tile::TileState::UI);
		tile->SetStateColor(Tile::TileState::UI);
	}
	selectedPreview.clear();

	for (int x = std::min(startPreviewIndex.x, endPreviewIndex.x); x <= std::max(startPreviewIndex.x, endPreviewIndex.x); ++x)
	{
		for (int y = std::min(startPreviewIndex.y, endPreviewIndex.y); y <= std::max(startPreviewIndex.y, endPreviewIndex.y); ++y)
		{
			tilesPreview[x][y]->SetState(Tile::TileState::SelectUI);
			tilesPreview[x][y]->SetStateColor(Tile::TileState::SelectUI);
			tilesPreview[x][y]->SetIndex(x, y);
			selectedPreview.push_back(tilesPreview[x][y]);

			//std::cout << "TileType : " << (int)tilesPreview[x][y]->GetType() << std::endl;
		}
	}
}

sf::Vector2i SceneEditor::GetCurrentPreviewIntIndex()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f UiMousePos = SCENE_MGR.GetCurrScene()->ScreenToUiPos(mousePos);

	int xIndex = static_cast<int>(UiMousePos.x - blankPos) / tileSize;
	int yIndex = static_cast<int>(UiMousePos.y - blankPos) / tileSize;

	int maxRangeIndex = atlasTextureSize / tileTextureSize;
	if (xIndex < 0 || xIndex >= maxRangeIndex || yIndex < 0 || yIndex >= maxRangeIndex)
	{
		xIndex = std::max(0, std::min(xIndex, maxRangeIndex - 1));
		yIndex = std::max(0, std::min(yIndex, maxRangeIndex - 1));
	}
	return sf::Vector2i(xIndex, yIndex);
}

void SceneEditor::SaveToCSV(const std::string& path)
{
	rapidcsv::Document doc;

	doc.SetColumnName(0,  "tileName");
	doc.SetColumnName(1,  "tileIndexX");
	doc.SetColumnName(2,  "tileIndexY");
	doc.SetColumnName(3,  "tileType");
	doc.SetColumnName(4,  "tileSize");
	doc.SetColumnName(5,  "tileScaleFactor");
	doc.SetColumnName(6,  "tileLayer");

	doc.SetColumnName(7,  "textureId");
	doc.SetColumnName(8,  "textureRectLeft");
	doc.SetColumnName(9,  "textureRectTop");
	doc.SetColumnName(10, "textureRectWidth");
	doc.SetColumnName(11, "textureRectHeight");

	for (int i = 0; i < tilesWorld.size(); i++)
	{
		for (int j = 0; j < tilesWorld[i].size(); j++)
		{
			Tile* tile = tilesWorld[i][j];
			if (tile) 
			{
				SpriteGo* atlas = (SpriteGo*)FindGo("AtlasPreview");
				doc.SetCell<std::string>("tileName", i * tilesWorld.size() + j, "Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")");
				doc.SetCell<int>("tileIndexX", i * tilesWorld[i].size() + j, i);
				doc.SetCell<int>("tileIndexY", i * tilesWorld.size() + j, j);
				doc.SetCell<int>("tileType", i * tilesWorld.size() + j, (int)tile->GetType());
				doc.SetCell<int>("tileSize", i * tilesWorld.size() + j, (int)tileSize);
				doc.SetCell<int>("tileScaleFactor", i * tilesWorld.size() + j, tileScaleFactor);
				doc.SetCell<int>("tileLayer", i * tilesWorld.size() + j, tile->GetLayer());

				sf::IntRect rect = tile->GetTextureRect();
				doc.SetCell<std::string>("textureId", i * tilesWorld.size() + j, atlas->textureId);
				doc.SetCell<int>("textureRectLeft", i * tilesWorld.size() + j, rect.left);
				doc.SetCell<int>("textureRectTop", i * tilesWorld.size() + j, rect.top);
				doc.SetCell<int>("textureRectWidth", i * tilesWorld.size() + j, rect.width);
				doc.SetCell<int>("textureRectHeight", i * tilesWorld.size() + j, rect.height);
			}
		}
	}
	doc.Save(path);
	std::cout << "SYSTEM : Save Success" << std::endl;
}

void SceneEditor::LoadFromCSV(const std::string& path)
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
		//tile->SetScale(tileScaleFactor);
		tile->SetLayer(tileLayer);
		tile->SetTexture(*RESOURCE_MGR.GetTexture(textureId));
		tile->SetTextureRect(textureRect, textureId);
		tile->SetOrigin(Origins::TL);
		tilesWorld[tileIndexX][tileIndexY] = tile;
	}
	std::cout << "SYSTEM : Load Success" << std::endl;
}

BaseUI* SceneEditor::CreateButton(const std::string& name, const std::string& text, float posX, float posY, float size, int texIndex, std::function<void()> onClickAction)
{
	//SpriteGo* newIcon = (SpriteGo*)AddGo(new SpriteGo(iconId, iconName));
	//newIcon->sortLayer = 111;

	BaseUI* newButton = (BaseUI*)AddGo(new BaseUI(name, UiType::Box));
	newButton->sortLayer = 110;
	newButton->SetPosition(posX, posY);
	newButton->SetColor(sf::Color::Green);
	newButton->SetSizeAdd(size, size);
	newButton->OnEnter = [newButton]()
	{
	};
	newButton->OnExit = [newButton]()
	{
	};
	newButton->OnClick = onClickAction;

	return newButton;
}