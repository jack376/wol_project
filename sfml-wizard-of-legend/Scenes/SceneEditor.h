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
	std::vector<std::vector<Tile*>> tiles;
	float tileSize = 16.0f;

	sf::Sprite camera;
	sf::Vector2f cameraDirection;
	sf::Vector2f cameraPosition;
	float cameraSpeed = 1000.0f;

	float refreshDelay = 1.0f;
	float resolutionScaleFactor = 1.0f;
	float zoomInFactor = 2.0f;
	float zoomOutFactor = 0.5f;

	bool isUiButtonActive = false;
	bool isMouseSelect = false;

	sf::Vector2i startTileIndex;
	sf::Vector2i endTileIndex;

public:
	SceneEditor();
	virtual ~SceneEditor() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	BaseUI* CreateButton(const std::string& name, const std::string& text, float posX, float posY, std::function<void()> onClickAction);

	Tile* CreateTile(const std::string& name, float posX, float posY);

	std::vector<Tile*> GetAllTiles();
	std::vector<Tile*> GetSelectedTiles();

	void SetSelectedTilesState(std::vector<Tile*>& selectedTiles, Tile::TileState state);
	void SetSelectedTilesArea();

	sf::Vector2i GetCurrentTileIntIndex();
};