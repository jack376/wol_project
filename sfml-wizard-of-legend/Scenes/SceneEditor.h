#pragma once
#include "Scene.h"
#include "Tile.h"

class TextGo;
class SpriteGo;
class BaseUI;
class Tile;

class SceneEditor : public Scene
{
protected:
	int rows = 32;
	int cols = 32;

	sf::Vector2f windowSize;
	float fhdWidth = 1920.0f;
	float blankPos = 32.0f;
	float resolutionScaleFactor = 1.0f;

	std::string textureId = "graphics/editor/FireTileSet.png";
	std::vector<std::vector<sf::IntRect>> tileTextureAtlas;
	int atlasTextureSize = 512;
	int tileTextureSize = 16;
	int tileScaleFactor = 4;

	std::vector<std::vector<Tile*>> tilesWorld;
	std::vector<std::vector<Tile*>> tilesPreview;
	float tileSize = 16.0f;

	std::vector<Tile*> allTiles;
	std::vector<Tile*> selectedTiles;
	sf::Vector2i startTileIndex;
	sf::Vector2i endTileIndex;
	bool isUiButtonActive = false;
	bool isWorldMouseSelect = false;

	std::vector<Tile*> allPreview;
	std::vector<Tile*> selectedPreview;
	sf::Vector2i startPreviewIndex;
	sf::Vector2i endPreviewIndex;
	bool isUiMouseSelect = false;

	sf::Sprite camera;
	sf::Vector2f cameraDirection;
	sf::Vector2f cameraPosition;
	float cameraSpeed = 1000.0f;

	float zoomDefaultFactor = 0.5f;
	float zoomInFactor = 0.5f;
	float zoomOutFactor = 2.0f;

	sf::Vector2i selectPaletteIndex;

public:
	SceneEditor();
	virtual ~SceneEditor() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	Tile* CreateTile(const std::string& name, float posX, float posY, int sort = 0);
	std::vector<Tile*> GetAllTiles();
	std::vector<Tile*> GetSelectedTiles();
	void SetSelectedTilesState(Tile::TileState state = Tile::TileState::Blank);
	void SetSelectedTilesType(Tile::TileType type);
	void SetSelectedTilesDraw();
	void SetSelectedTilesArea();
	sf::Vector2i GetCurrentTileIntIndex();

	Tile* CreateTilePreview(const std::string& name, float posX, float posY, int sort = 100);
	void SetSelectedPreviewState(Tile::TileState state);
	void SetSelectedPreviewArea();
	sf::Vector2i GetCurrentPreviewIntIndex();

	void SaveToCSV(const std::string& path);
	void LoadFromCSV(const std::string& path);

	BaseUI* CreateButton(const std::string& name, const std::string& text, float posX, float posY, float size, int texIndex, std::function<void()> onClickAction);
};