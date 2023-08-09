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
	sf::Vector2f windowSize;
	float fhdWidth = 1920.0f;
	float blankPos = 32.0f;
	float resolutionScaleFactor = 1.0f;

	std::vector<std::vector<sf::IntRect>> tileTextureAtlas;
	int atlasTextureSize = 512;
	int tileTextureSize = 16;

	std::vector<std::vector<Tile*>> tilesWorld;
	std::vector<std::vector<Tile*>> tilesUi;
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
	float zoomInFactor = 0.5f;
	float zoomOutFactor = 2.0f;

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
	void SetSelectedTilesState(std::vector<Tile*>& selectedTiles, Tile::TileState state);
	void SetSelectedTilesRect(std::vector<Tile*>& selectedTiles, const sf::IntRect& rect);
	void SetSelectedTilesArea();
	sf::Vector2i GetCurrentTileIntIndex();

	Tile* CreateTilePreview(const std::string& name, float posX, float posY, int sort = 100);
	std::vector<Tile*> GetAllPreview();
	std::vector<Tile*> GetSelectedPreview();
	void SetSelectedPreviewState(std::vector<Tile*>& selectedPreview, Tile::TileState state);
	void SetSelectedPreviewRect(std::vector<Tile*>& selectedPreview, const sf::IntRect& rect);
	void SetSelectedPreviewArea();
	sf::Vector2i GetCurrentPreviewIntIndex();
	int ToOneDimensionIndex(int row, int col, int numCols);
};