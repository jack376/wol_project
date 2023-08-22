#pragma once
#include "Tile.h"
#include "Scene.h"
#include "ObjectPool.h"
#include "BoxCollider2D.h"

class UIButton;
class TextGo;
class SpriteGo;
class Tile;

class Player;
class Monster;
class ElementalSpell;
class BreakableObj;
class Particle;
class Monster;
class CustomEffect;
enum class MonsterId;

using Pair = std::pair<int, int>;


class PathfindingScene :
    public Scene
{
protected:
	std::vector<std::vector<Tile*>> tilesWorld;
	std::vector<std::vector<int>> intMap;
	std::pair<bool, std::stack<Pair>> path;

	int rows = 0;
	int cols = 0;
	float tileSize = 64.0f;

	sf::Sprite camera;
	sf::Vector2f cameraDirection;
	sf::Vector2f cameraPosition;
	float cameraSpeed = 1000.0f;

	float zoomDefaultFactor = 0.5f;
	float zoomInFactor = 0.5f;
	float zoomOutFactor = 2.0f;

public:
	PathfindingScene();
	virtual ~PathfindingScene() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	Tile* CreateTile(const std::string& name, float posX, float posY, int sort = 0);
	void LoadFromCSV(const std::string& path);
	void CreateTile2dVector(int rows, int cols);

	void TilesToIntMap();
};

