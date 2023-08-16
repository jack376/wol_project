#pragma once
#include "Tile.h"
#include "Scene.h"
#include "ObjectPool.h"

class UIButton;
class TextGo;
class SpriteGo;
class Tile;

class Player;
class ElementalSpell;
class BreakableObj;
class Particle;

class SceneGame : public Scene
{
protected:
	Player* player;
	ElementalSpell* tempWindSlash;
	std::vector<std::vector<Tile*>> tilesWorld;

	int rows = 0;
	int cols = 0;
	float tileSize = 64.0f;

	ObjectPool<Particle> particlePool;

	int count = 0;

public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	template <typename T>
	void ClearObjectPool(ObjectPool<T>& pool);

	Player* GetPlayer() { return player; }

	Tile* CreateTile(const std::string& name, float posX, float posY, int sort = 0);
	void LoadFromCSV(const std::string& path);
	void CreateTile2dVector(int rows, int cols);
	void CreateParticle(int count);

	void SpawnBreakableObj(const std::string& id, int count);
	void DestroyBreakableObj(BreakableObj* obj);

	std::vector<sf::Vector2i> CreateRandomPath(Tile::TileType startType, Tile::TileType traversableType, int pathLength);
	void TestRandomPath();
	
	std::vector<std::vector<sf::Vector2i>> CreateMultipleRandomPaths(Tile::TileType startType, Tile::TileType traversableType, int mainPathLength, int branchCount = 2, int branchLength = 10);
	void TestMultipleRandomPaths();
};

