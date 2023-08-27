#include "stdafx.h"
#include "Framework.h"
#include "SceneMgr.h"
#include "InputMgr.h"
#include "ResourceMgr.h"
#include "GameObject.h"
#include "SceneEditor.h"
#include "TextGo.h"
#include "SpriteGo.h"
#include "DecoGo.h"
#include "BaseUI.h"
#include "Tile.h"
#include "Particle.h"
#include "rapidcsv.h"

SceneEditor::SceneEditor() : Scene(SceneId::Editor)
{
}

void SceneEditor::Init()
{
	Release();

	windowSize = FRAMEWORK.GetWindowSize();
	resolutionScaleFactor = windowSize.x / fhdWidth;


	// Texture Atlas
	tileAtlasPreview = (SpriteGo*)AddGo(new SpriteGo("graphics/editor/FireTileSet.png", "TileAtlasPreview"));
	tileAtlasPreview->sortLayer = 106;
	tileAtlasPreview->SetOrigin(Origins::TL);
	tileAtlasPreview->SetPosition(blankPos, blankPos);
	tileAtlasPreview->SetActive(true);

	decoAtlasPreview = (SpriteGo*)AddGo(new SpriteGo("graphics/editor/DecoSet.png", "DecoAtlasPreview"));
	decoAtlasPreview->sortLayer = 105;
	decoAtlasPreview->SetOrigin(Origins::TL);
	decoAtlasPreview->SetPosition(blankPos, blankPos);
	decoAtlasPreview->SetActive(false);

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

	// UI backgorund
	BaseUI* uiBackground = (BaseUI*)AddGo(new BaseUI("UiBackGround", UiType::Box));
	uiBackground->sortLayer = 101;
	uiBackground->SetSizeAdd(atlasTextureSize + blankPos * 2.0f, windowSize.y);
	uiBackground->SetColor(sf::Color(16, 16, 16, 255));
	uiBackground->SetStrokeColor(sf::Color(32, 32, 64, 255));

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
}

void SceneEditor::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}

Tile* SceneEditor::CreateTile(const std::string& name, float posX, float posY, int sort)
{
	Tile* tile = (Tile*)AddWithoutCheckGo(new Tile(name));
	tile->sortLayer = sort;
	tile->SetPosition(posX, posY);
	tile->SetState();
	tile->SetStateColor(Tile::TileState::Blank);
	tile->SetStrokeColor(sf::Color(64, 64, 64, 96));
	tile->OnEnter = [tile, this]()
	{
		if (tile->GetState() == Tile::TileState::Blank)
		{
			tile->SetStateColor(Tile::TileState::Select);
		}
	};
	tile->OnExit = [tile, this]()
	{
		if (tile->GetState() == Tile::TileState::Blank)
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
		if (tile->GetState() == Tile::TileState::Blank)
		{
			startTileIndex = GetCurrentTileIntIndex();
			isUiMouseSelect = false;
			isWorldMouseSelect = true;
		}
	};
	tile->OnClickDrag = [tile, this]()
	{
		if (isWorldMouseSelect && tile->GetState() == Tile::TileState::Blank)
		{
			endTileIndex = GetCurrentTileIntIndex();
			SetSelectedTilesArea();
		}
	};
	tile->OnClickUp = [tile, this]()
	{
		if (isWorldMouseSelect && tile->GetState() == Tile::TileState::Blank)
		{
			endTileIndex = GetCurrentTileIntIndex();
			isUiMouseSelect = true;
			isWorldMouseSelect = false;
		}
	};
	return tile;
}

void SceneEditor::CreateTile2dVector(int rows, int cols)
{
	tilesWorld.resize(rows, std::vector<Tile*>(cols, nullptr));
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Tile* tile = CreateTile("Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")", i * tileSize, j * tileSize);
			tilesWorld[i][j] = tile;
			tilesWorld[i][j]->SetIndex(i, j);
			//std::cout << "Loading : Tile(" + std::to_string(i) + ", " + std::to_string(j) + ")" << std::endl;
		}
	}
}

std::vector<Tile*> SceneEditor::GetAllTiles()
{
	for (auto& tiles : tilesWorld)
	{
		for (Tile* tile : tiles)
		{
			allTiles.push_back(tile);
		}
	}
	return allTiles;
}

std::vector<Tile*> SceneEditor::GetSelectedTiles()
{
	for (auto& tiles : tilesWorld) 
	{
		for (Tile* tile : tiles) 
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

void SceneEditor::SetSelectedTilesType(TileType type)
{
	for (Tile* tile : selectedTiles)
	{
		tile->SetType(type);
		tile->SetTypeColor(type);
	}
	SetSelectedTilesState();
}

void SceneEditor::SetSelectedSpawnLocation(SpawnLocation spawn)
{
	for (Tile* tile : selectedTiles)
	{
		tile->SetSpawnLocation(spawn);
		tile->SetSpawnLocationColor(spawn);
	}
	SetSelectedTilesState();
}

void SceneEditor::SetSelectedTilesDraw()
{
	if (selectedTiles.empty() || selectedPreview.empty())
	{
		return;
	}
	currentCommandId++;
	std::vector<std::unique_ptr<Command>> tileCommands;

	sf::Vector2i indexPreviewStart = selectedPreview[0]->GetIndex();
	int previewWidth  = std::abs(endPreviewIndex.x - startPreviewIndex.x) + 1;
	int previewHeight = std::abs(endPreviewIndex.y - startPreviewIndex.y) + 1;
	sf::Vector2i indexTilesStart = selectedTiles[0]->GetIndex();

	for (Tile* worldTile : selectedTiles)
	{
		TileCommand::TileState before = CaptureTileState(worldTile);

		sf::Vector2i indexWorld = worldTile->GetIndex();
		int offsetX = (indexWorld.x - indexTilesStart.x) % previewWidth;
		int offsetY = (indexWorld.y - indexTilesStart.y) % previewHeight;
		sf::Vector2i matchPreviewIndex(indexPreviewStart.x + offsetX, indexPreviewStart.y + offsetY);

		Tile* previewTile = tilesPreview[matchPreviewIndex.x][matchPreviewIndex.y];

		sf::IntRect previewRect = sf::IntRect(previewTile->GetIndex().x * tileSize, previewTile->GetIndex().y * tileSize, tileSize, tileSize);

		isTileLayer ? worldTile->SetTextureRectTop(previewRect, tileTextureId) : worldTile->SetTextureRectBottom(previewRect, tileTextureId);

		TileCommand::TileState after = CaptureTileState(worldTile);

		std::unique_ptr<Command> command = std::make_unique<TileCommand>(worldTile, before, after);
		tileCommands.push_back(std::move(command));
	}
	commandInvoker.Execute(std::move(tileCommands), currentCommandId);

	SetSelectedTilesState(Tile::TileState::Blank);
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
		}
	}
}

sf::Vector2i SceneEditor::GetCurrentTileIntIndex()
{
	sf::Vector2f mousePos = INPUT_MGR.GetMousePos();
	sf::Vector2f worldMousePos = SCENE_MGR.GetCurrScene()->ScreenToWorldPos(mousePos);

	int xIndex = static_cast<int>(worldMousePos.x) / tileSize;
	int yIndex = static_cast<int>(worldMousePos.y) / tileSize;

	if (xIndex < 0 || xIndex >= rows || yIndex < 0 || yIndex >= cols)
	{
		xIndex = std::max(0, std::min(xIndex, rows - 1));
		yIndex = std::max(0, std::min(yIndex, cols - 1));
	}

	std::cout << "World Index : (" << xIndex << ", " << yIndex << ")" << std::endl;

	return sf::Vector2i(xIndex, yIndex);
}

Tile* SceneEditor::CreateTilePreview(const std::string& name, float posX, float posY, int sort)
{
	Tile* tilePreview = (Tile*)AddWithoutCheckGo(new Tile(name));
	tilePreview->sortLayer = sort;
	tilePreview->SetPosition(posX + blankPos, posY + blankPos);
	tilePreview->SetState(Tile::TileState::UI);
	tilePreview->SetType(TileType::None);
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

	std::cout << "Preview Index : (" << xIndex << ", " << yIndex << ")" << std::endl;

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
	doc.SetColumnName(5,  "tileScale");
	doc.SetColumnName(6,  "tileLayer");

	doc.SetColumnName(7,  "textureId");
	doc.SetColumnName(8,  "topTextureRectL");
	doc.SetColumnName(9,  "topTextureRectT");
	doc.SetColumnName(10, "bottomTextureRectL");
	doc.SetColumnName(11, "bottomTextureRectT");
	doc.SetColumnName(12, "spawnLocation");

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			Tile* tile = tilesWorld[i][j];
			if (tile) 
			{
				SpriteGo* atlas = (SpriteGo*)FindGo("AtlasPreview");
				doc.SetCell<std::string>("tileName", i * cols + j, tile->GetName());

				doc.SetCell<int>("tileIndexX", i * cols + j, i);
				doc.SetCell<int>("tileIndexY", i * cols + j, j);
				doc.SetCell<int>("tileType",   i * cols + j, static_cast<int>(tile->GetType()));
				doc.SetCell<int>("tileSize",   i * cols + j, static_cast<int>(tileSize));
				doc.SetCell<int>("tileScale",  i * cols + j, tileScaleFactor);
				doc.SetCell<int>("tileLayer",  i * cols + j, tile->GetLayer());

				doc.SetCell<std::string>("textureId", i * cols + j, tileTextureId);

				sf::IntRect topRect = tile->GetTextureRectTop();
				doc.SetCell<int>("topTextureRectL", i * cols + j, topRect.left);
				doc.SetCell<int>("topTextureRectT", i * cols + j, topRect.top);

				sf::IntRect bottomRect = tile->GetTextureRectBottom();
				doc.SetCell<int>("bottomTextureRectL", i * cols + j, bottomRect.left);
				doc.SetCell<int>("bottomTextureRectT", i * cols + j, bottomRect.top);

				doc.SetCell<int>("spawnLocation", i * cols + j, static_cast<int>(tile->GetSpawnLocation()));
			}
		}
	}
	doc.Save(path);
	//std::cout << "SYSTEM : Save Success" << std::endl;
}

void SceneEditor::LoadFromCSV(const std::string& path)
{
	for (auto& row : tilesWorld)
	{
		for (Tile* tile : row)
		{
			if (tile)
			{
				//RemoveGo(tile);
				gameObjects.remove(tile);
				delete tile;
			}
		}
		row.clear();
	}
	tilesWorld.clear();
	 
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

		tileTextureId = doc.GetCell<std::string>("textureId", i);

		sf::IntRect topTextureRect(doc.GetCell<int>("topTextureRectL", i), doc.GetCell<int>("topTextureRectT", i), tileSize, tileSize);
		sf::IntRect bottomTextureRect(doc.GetCell<int>("bottomTextureRectL", i), doc.GetCell<int>("bottomTextureRectT", i), tileSize, tileSize);

		int spawnLocation = doc.GetCell<int>("spawnLocation", i);

		Tile* tile = (Tile*)FindGo(tileName);
		tile->SetIndex(tileIndexX, tileIndexY);
		tile->SetType(static_cast<TileType>(tileType));
		tile->SetTileSize(tileSize);
		tile->SetLayer(tileLayer);
		tile->SetTexture(tileTextureId);
		tile->SetTextureRectTop(topTextureRect, tileTextureId);
		tile->SetTextureRectBottom(bottomTextureRect, tileTextureId);
		tile->SetSpawnLocation(static_cast<SpawnLocation>(spawnLocation));
		tile->SetOrigin(Origins::TL);
		tilesWorld[tileIndexX][tileIndexY] = tile;
	}
	//std::cout << "SYSTEM : Editor Tile Load Success" << std::endl;
}

BaseUI* SceneEditor::CreateButton(const std::string& name, const std::string& text, float posX, float posY, float size, int texIndex, std::function<void()> onClickAction)
{
	BaseUI* newButton = (BaseUI*)AddGo(new BaseUI(name, UiType::Box));
	newButton->sortLayer = 115;
	newButton->SetPosition(posX, posY);
	newButton->SetColor(sf::Color(32, 32, 64, 255));
	newButton->SetSizeAdd(size + 56.0f, size - 16.0f);
	//newButton->SetStrokeColor(sf::Color(64, 64, 128, 128));
	newButton->OnEnter = [newButton]()
	{
		newButton->SetColor(sf::Color::Color(64, 64, 128, 255));
	};
	newButton->OnExit = [newButton]()
	{
		newButton->SetColor(sf::Color::Color(32, 32, 64, 255));
	};
	newButton->OnClick = onClickAction;

	TextGo* buttonText = (TextGo*)AddGo(new TextGo("fonts/NanumSquareEB.ttf", name + "T"));
	buttonText->sortLayer = 116;
	buttonText->text.setCharacterSize(16);
	buttonText->text.setPosition(posX + 60.0f, posY + 20.0f);
	buttonText->text.setFillColor(sf::Color::White);
	buttonText->text.setString(text);
	buttonText->SetOrigin(Origins::MC);

	return newButton;
}

TileCommand::TileState SceneEditor::CaptureTileState(const Tile* tile)
{
	TileCommand::TileState state;

	state.textureId  = tileTextureId;
	state.type       = tile-> GetType();
	state.index      = tile-> GetIndex();
	state.topRect    = tile-> GetTextureRectTop();
	state.bottomRect = tile-> GetTextureRectBottom();
	state.size       = tile-> GetTileSize();
	state.layer      = tile-> GetLayer();

	return state;
}

void SceneEditor::ApplyTileState(Tile* tile, const TileCommand::TileState& state)
{
	tile->SetTexture(state.textureId);
	tile->SetType(state.type);
	tile->SetIndex(state.index.x, state.index.y);
	tile->SetTextureRectTop(state.topRect, state.textureId);
	tile->SetTextureRectBottom(state.bottomRect, state.textureId);
	tile->SetTileSize(state.size);
	tile->SetLayer(state.layer);
}

void SceneEditor::CopySelectedTiles()
{
	clipboardTiles.clear();

	if (selectedTiles.empty())
	{
		return;
	}

	for (Tile* worldTile : selectedTiles)
	{
		TileCommand::TileState tileState = CaptureTileState(worldTile);
		clipboardTiles.push_back(tileState);
	}
	SetSelectedTilesState(Tile::TileState::Blank);
}

void SceneEditor::PasteSelectedTiles()
{
	if (selectedTiles.empty() || clipboardTiles.empty())
	{
		return;
	}

	currentCommandId++;
	std::vector<std::unique_ptr<Command>> tileCommands;

	sf::Vector2i indexGrid = selectedTiles[0]->GetIndex();
	sf::Vector2i indexWorldTile = clipboardTiles[0].index;

	for (size_t i = 0; i < clipboardTiles.size(); i++)
	{
		sf::Vector2i currentCopyIndex = clipboardTiles[i].index; 
		sf::Vector2i offset = currentCopyIndex - indexWorldTile;

		int indexX = indexGrid.x + offset.x;
		int indexY = indexGrid.y + offset.y;
		if (indexX < 0 || indexX >= rows || indexY < 0 || indexY >= cols)
		{
			continue;
		}

		Tile* worldTile = tilesWorld[indexX][indexY];
		if (!worldTile)
		{
			continue;
		}
	
		TileCommand::TileState before = CaptureTileState(worldTile);
		ApplyTileState(worldTile, clipboardTiles[i]);
		TileCommand::TileState after = CaptureTileState(worldTile);

		std::unique_ptr<Command> command = std::make_unique<TileCommand>(worldTile, before, after);
		tileCommands.push_back(std::move(command));
	}
	commandInvoker.Execute(std::move(tileCommands), currentCommandId);

	SetSelectedTilesState(Tile::TileState::Blank);
}

void SceneEditor::ResizeWorld(int newRows, int newCols)
{
	TempWorld();
	ClearWorld();
	tilesWorld.resize(newRows, std::vector<Tile*>(newCols, nullptr));

	rows = newRows;
	cols = newCols;
	CreateTile2dVector(rows, cols);

	startTileIndex = sf::Vector2i(0, 0);
	endTileIndex = sf::Vector2i(0, 0);
	SetSelectedTilesArea();
	PasteSelectedTiles();
}

void SceneEditor::TempWorld()
{
	if (!tilesWorld.empty() && !tilesWorld[0].empty())
	{
		Tile* firstTile = tilesWorld[0][0];
		if (firstTile != nullptr)
		{
			clipboardTiles.clear();
			if (tilesWorld.empty())
			{
				return;
			}
			for (auto& tiles : tilesWorld)
			{
				for (Tile* tile : tiles)
				{
					TileCommand::TileState tileState = CaptureTileState(tile);
					clipboardTiles.push_back(tileState);
				}
			}
			SetSelectedTilesState(Tile::TileState::Blank);
		}
		else
		{
			std::cout << "ERROR : failed copy, first tile nullptr" << std::endl;
		}
	}
	else
	{
		std::cout << "REPORT : tilesWorld is empty & first sub-vector is empty" << std::endl;
	}
}

void SceneEditor::ClearWorld()
{
	for (int i = 0; i < tilesWorld.size(); i++)
	{
		for (int j = 0; j < tilesWorld[i].size(); j++)
		{
			if (tilesWorld[i][j] != nullptr)
			{
				tilesWorld[i][j]->SetActive(false);
				//RemoveGo(tilesWorld[i][j]);
				gameObjects.remove(tilesWorld[i][j]);
			}
		}
	}
	tilesWorld.clear();
	tilesWorld.resize(0, std::vector<Tile*>(0, nullptr));
}

void SceneEditor::DrawEditorUI()
{
	/*
	ImGui::Begin("StyleEditor");
	ImGui::ShowStyleEditor();
	ImGui::End();
	*/

	// UI Style
	ImGui::StyleColorsClassic();
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowPadding.x = 0.0f;
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 0.0f;
	style.FrameRounding = 4.0f;
	style.FramePadding.x = 8.0f;
	style.FramePadding.y = 4.0f;
	style.ItemSpacing.x = 4.0f;
	style.ItemSpacing.y = 6.0f;

	// UI Window Key Shotcut
	bool showWindow = false;
	ImGui::SetNextWindowPos(ImVec2(windowSize.x - 256.0f - blankPos, blankPos));
	ImGui::SetNextWindowSize(ImVec2(256.0f, 640.0f));
	ImGui::Begin("KEY SHOTCUTS GUIDE", &showWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::LabelText("MOUSE L", "SELECT");
	ImGui::LabelText("MOUSE R", "DESELECT");
	ImGui::Separator();
	ImGui::LabelText("W,A,S,D", "MOVE");
	ImGui::LabelText("Q", "DRAW");
	ImGui::LabelText("E", "TOP LAYER");
	ImGui::LabelText("R", "BOTTOM LAYER");
	ImGui::Separator();
	ImGui::LabelText("T", "GRID VIEW");
	ImGui::LabelText("Y", "TILE OVERLAY");
	ImGui::LabelText("Z", "ZOOM IN");
	ImGui::LabelText("X", "ZOOM OUT");
	ImGui::Separator();
	ImGui::LabelText("1", "GROUND");
	ImGui::LabelText("2", "CLIFF");
	ImGui::LabelText("3", "WALL");
	ImGui::LabelText("4", "NONE");
	ImGui::Separator();
	ImGui::LabelText("Ctrl+Z", "UNDO");
	ImGui::LabelText("Ctrl+X", "REDO");
	ImGui::LabelText("Ctrl+C", "COPY");
	ImGui::LabelText("Ctrl+V", "PASTE");
	ImGui::LabelText("Ctrl+S", "SAVE");
	ImGui::End();

	// UI Window Top
	float windowTopSizeY = blankPos * 1.5f;
	ImGui::SetNextWindowPos(ImVec2(blankPos, atlasTextureSize + blankPos * 2.0f));
	ImGui::SetNextWindowSize(ImVec2(atlasTextureSize + blankPos * 2.0f, windowTopSizeY));
	ImGui::Begin("Top UI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

	std::vector<std::string> labels = { "DRAW (Q)", "GROUND (1)", "CLIFF (2)", "WALL (3)", "NONE (4)" };
	std::vector<int> colors = { 5, 2, 0, 1, 4 };
	for (int i = 0; i < 5; i++)
	{
		if (i > 0)
		{
			ImGui::SameLine();
		}
		ImGui::PushID(i);
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(colors[i] / 7.0f, 0.5f, 0.3f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(colors[i] / 7.0f, 0.6f, 0.4f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(colors[i] / 7.0f, 0.7f, 0.5f));
		if (ImGui::Button(labels[i].c_str()))
		{
			switch (i)
			{
			case 0:
				SetSelectedTilesDraw();
				break;
			case 1:
				SetSelectedTilesType(TileType::Ground);
				break;
			case 2:
				SetSelectedTilesType(TileType::Cliff);
				break;
			case 3:
				SetSelectedTilesType(TileType::Wall);
				break;
			case 4:
				SetSelectedTilesType(TileType::None);
				break;
			default:
				break;
			}
		}
		ImGui::PopStyleColor(3);
		ImGui::PopID();
	}
	ImGui::SameLine(0.0f, 8.0f);

	// Texture Atlas Drobbox
	const char* environment[] = { "DungeonTile", "DecoObject" };
	static int environmentIndex = 0;
	ImGui::SetNextItemWidth(128.0f);
	ImGui::Combo("##input6", &environmentIndex, environment, IM_ARRAYSIZE(environment));

	switch (environmentIndex)
	{
	case 0:
		tileAtlasPreview->SetActive(true);
		decoAtlasPreview->SetActive(false);
		break;
	case 1:
		tileAtlasPreview->SetActive(false);
		decoAtlasPreview->SetActive(true);
		break;
	default:
		tileAtlasPreview->SetActive(true);
		decoAtlasPreview->SetActive(false);
		break;
	}

	ImGui::AlignTextToFramePadding();
	ImGui::End();

	// UI Window Left
	ImGui::SetNextWindowPos(ImVec2(blankPos, atlasTextureSize + blankPos * 2.0f + windowTopSizeY));
	ImGui::SetNextWindowSize(ImVec2(atlasTextureSize / 2, windowSize.y - (atlasTextureSize + blankPos * 2.0f)));
	ImGui::Begin("LEFT UI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	// Save
	static char maxPathLengthSave[128] = "";
	ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.7f, 1.0f), "SAVE CSV");
	ImGui::SetNextItemWidth(192.0f);
	ImGui::InputTextWithHint("##input1", u8"파일명 입력 CSV 생략", maxPathLengthSave, IM_ARRAYSIZE(maxPathLengthSave));
	ImGui::SameLine();
	if (ImGui::Button("SAVE") || INPUT_MGR.GetKeyDown(sf::Keyboard::S) && INPUT_MGR.GetKey(sf::Keyboard::LControl))
	{
		std::string str = maxPathLengthSave;
		auto now = std::chrono::system_clock::now();
		std::time_t now_time = std::chrono::system_clock::to_time_t(now);

		std::stringstream ss;
		ss << std::put_time(std::localtime(&now_time), "_%m%d_%H%M%S"); // e.g., "_0820_143059"
		std::string timestamp = ss.str();
		std::string filename = "tables/" + str + timestamp + ".csv";

		try
		{
			SaveToCSV(filename);
			std::cout << "Successfully saved CSV file : " << filename << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << "ERROR : Error occurred while saving CSV file." << e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "ERROR : Exception occurred during the save." << std::endl;
		}
	}
	ImGui::Dummy(ImVec2(0.0f, 8.0f));

	// Map Size Modify
	ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.7f, 1.0f), "MAP SIZE MODIFY");
	const char* mapSizeX[] = { "16", "24", "32", "48", "64", "96", "128", "192", "256" };
	static int mapSizeIndexX = 0;
	ImGui::SetNextItemWidth(64.0f);
	ImGui::Combo("##input4", &mapSizeIndexX, mapSizeX, IM_ARRAYSIZE(mapSizeX));
	ImGui::SameLine();
	const char* mapSizeY[] = { "16", "24", "32", "48", "64", "96", "128", "192", "256" };
	static int mapSizeIndexY = 0;
	ImGui::SetNextItemWidth(64.0f);
	ImGui::Combo("##input5", &mapSizeIndexY, mapSizeY, IM_ARRAYSIZE(mapSizeY));
	ImGui::SameLine();
	if (ImGui::Button("OK"))
	{
		int width, height;
		sscanf(mapSizeX[mapSizeIndexX], "%d", &width);
		sscanf(mapSizeY[mapSizeIndexY], "%d", &height);
		ResizeWorld(width, height);
	}
	ImGui::Dummy(ImVec2(0.0f, 8.0f));

	// Edit
	ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.7f, 1.0f), "EDIT TOOL");
	if (ImGui::Button("UNDO"))
	{
		commandInvoker.Undo();
	}
	ImGui::SameLine();
	if (ImGui::Button("REDO"))
	{
		commandInvoker.Redo();
	}
	ImGui::SameLine();
	if (ImGui::Button("COPY"))
	{
		CopySelectedTiles();
	}
	ImGui::SameLine();
	if (ImGui::Button("PASTE"))
	{
		PasteSelectedTiles();
	}
	ImGui::AlignTextToFramePadding();
	ImGui::End();

	// UI Window Right
	ImGui::SetNextWindowPos(ImVec2(blankPos * 1.25f + atlasTextureSize / 2, atlasTextureSize + blankPos * 2.0f + windowTopSizeY));
	ImGui::SetNextWindowSize(ImVec2(atlasTextureSize / 2, windowSize.y - (atlasTextureSize + blankPos * 2.0f)));
	ImGui::Begin("RIGHT UI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	// Load
	static char maxPathLengthLoad[128] = "";
	ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.7f, 1.0f), "LOAD CSV");
	ImGui::SetNextItemWidth(192.0f);
	ImGui::InputTextWithHint("##input2", u8"파일명 입력 CSV 생략", maxPathLengthLoad, IM_ARRAYSIZE(maxPathLengthLoad));
	ImGui::SameLine();
	if (ImGui::Button("LOAD"))
	{
		std::string str = maxPathLengthLoad;
		try
		{
			LoadFromCSV("tables/" + str + ".csv");
			std::cout << "CSV file has been loaded successfully." << std::endl;
		}
		catch (const std::exception& e)
		{
			std::cout << "ERROR : Error while loading CSV file." << e.what() << std::endl;
		}
		catch (...)
		{
			std::cout << "ERROR : Exception encountered while loading." << std::endl;
		}
	}

	ImGui::Dummy(ImVec2(0.0f, 8.0f));

	// View
	static int layerView = 0;
	Scene* scene = SCENE_MGR.GetCurrScene();
	ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.7f, 1.0f), "LAVER TOGGLE");
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::E) || ImGui::RadioButton("LAYER TOP", &layerView, 0))
	{
		isTileLayer = true;
		std::cout << "Current Layer : Top" << std::endl;
		layerView = 0;
	}
	ImGui::SameLine();
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::R) || ImGui::RadioButton("LAYER BOTTOM", &layerView, 1))
	{

		isTileLayer = false;
		std::cout << "Current Layer : Bottom" << std::endl;
		layerView = 1;
	}
	ImGui::Dummy(ImVec2(0.0f, 8.0f));

	// Zoom
	ImGui::TextColored(ImVec4(0.4f, 0.5f, 0.7f, 1.0f), "VIEW OPTION");
	if (ImGui::Button("ZOOM IN"))
	{
		scene->Zoom(zoomInFactor);
	}
	ImGui::SameLine();
	if (ImGui::Button("ZOOM OUT"))
	{
		scene->Zoom(zoomOutFactor);
	}
	ImGui::AlignTextToFramePadding();
	ImGui::End();

	// UI Window Top
	ImGui::SetNextWindowPos(ImVec2(blankPos * 3.0f + atlasTextureSize, 24.0f));
	ImGui::SetNextWindowSize(ImVec2(700.0f, blankPos));
	ImGui::Begin("TOP UI", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);

	ImGui::ColorButton("NONE",   ImVec4(0.5f, 0.5f, 0.5f, 0.2f)); ImGui::SameLine(); ImGui::Text("NONE");   ImGui::SameLine(0.0f, 16.0f);
	ImGui::ColorButton("LAVER",  ImVec4(0.8f, 0.2f, 0.2f, 1.0f)); ImGui::SameLine(); ImGui::Text("LAVER");  ImGui::SameLine(0.0f, 16.0f);
	ImGui::ColorButton("OBJECT", ImVec4(1.0f, 1.0f, 0.0f, 1.0f)); ImGui::SameLine(); ImGui::Text("OBJECT"); ImGui::SameLine(0.0f, 16.0f);
	ImGui::ColorButton("WALL",   ImVec4(0.0f, 0.6f, 1.0f, 1.0f)); ImGui::SameLine(); ImGui::Text("WALL");   ImGui::SameLine(0.0f, 16.0f);
	ImGui::ColorButton("TORCH",  ImVec4(1.0f, 0.2f, 0.7f, 1.0f)); ImGui::SameLine(); ImGui::Text("TORCH");  ImGui::SameLine(0.0f, 16.0f);
	ImGui::ColorButton("ROOF",   ImVec4(0.5f, 0.2f, 0.8f, 1.0f)); ImGui::SameLine(); ImGui::Text("ROOF");   ImGui::SameLine(0.0f, 16.0f);
	ImGui::ColorButton("GLOW",   ImVec4(1.0f, 0.5f, 0.5f, 1.0f)); ImGui::SameLine(); ImGui::Text("GLOW");   ImGui::SameLine(0.0f, 16.0f);
	ImGui::ColorButton("PORTAL", ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); ImGui::SameLine(); ImGui::Text("PORTAL");
	ImGui::End();
}

void SceneEditor::InputEditorUI()
{
	// Zoom
	Scene* scene = SCENE_MGR.GetCurrScene();
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Z) && !INPUT_MGR.GetKey(sf::Keyboard::LControl)) { scene-> Zoom(zoomInFactor);  }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::X) && !INPUT_MGR.GetKey(sf::Keyboard::LControl)) { scene-> Zoom(zoomOutFactor); }

	// Copy, Paste
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::C) && INPUT_MGR.GetKey(sf::Keyboard::LControl)) { CopySelectedTiles();  }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::V) && INPUT_MGR.GetKey(sf::Keyboard::LControl)) { PasteSelectedTiles(); }

	// Undo, Redo
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Z) && INPUT_MGR.GetKey(sf::Keyboard::LControl)) { commandInvoker.Undo(); }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::X) && INPUT_MGR.GetKey(sf::Keyboard::LControl)) { commandInvoker.Redo(); }

	// Draw
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Q)) { SetSelectedTilesDraw(); }

	// SetSpawnLocation
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F1)) { SetSelectedSpawnLocation(SpawnLocation::None);       }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F2)) { SetSelectedSpawnLocation(SpawnLocation::Monster);    }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F3)) { SetSelectedSpawnLocation(SpawnLocation::Object);     }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F4)) { SetSelectedSpawnLocation(SpawnLocation::WallObject); }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F5)) { SetSelectedSpawnLocation(SpawnLocation::Torch);      }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F6)) { SetSelectedSpawnLocation(SpawnLocation::Roof);       }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F7)) { SetSelectedSpawnLocation(SpawnLocation::FireLight);  }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::F8)) { SetSelectedSpawnLocation(SpawnLocation::Portal);     }

	// SetTileType
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num1)) { SetSelectedTilesType(TileType::Ground); }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num2)) { SetSelectedTilesType(TileType::Cliff);  }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num3)) { SetSelectedTilesType(TileType::Wall);   }
	if (INPUT_MGR.GetKeyDown(sf::Keyboard::Num4)) { SetSelectedTilesType(TileType::None);   }

	// Deselect
	if (INPUT_MGR.GetMouseButtonDown(sf::Mouse::Right)) { SetSelectedTilesState(Tile::TileState::Blank); }
}

void SceneEditor::CemeraEditorUI(float dt)
{
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
}