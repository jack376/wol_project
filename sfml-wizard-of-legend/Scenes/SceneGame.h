#pragma once
#include "Scene.h"
#include "ObjectPool.h"

class UIButton;
class TextGo;
class SpriteGo;
class Tile;

class Player;
class ElementalSpell;
class DestructibleGo;
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

	sf::Sprite camera;
	sf::Vector2f cameraDirection;
	sf::Vector2f cameraPosition;
	float cameraSpeed = 400.0f;

	ObjectPool<Particle> particlePool;

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
};

