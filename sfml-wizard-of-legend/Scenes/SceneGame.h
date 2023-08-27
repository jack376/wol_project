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
class DecoGo;
class GlowGo;
class RoofGo;
class Particle;
class Monster;
class CustomEffect;
enum class MonsterId;

class SceneGame : public Scene
{
protected:
	Player* player;
	ElementalSpell* tempWindSlash;
	ElementalSpell* tempFireBall;
	std::vector<std::vector<Tile*>> tilesWorld;
	std::vector<Tile*> nongroundTiles;
	std::vector<std::vector<int>> intMap;

	int rows = 0;
	int cols = 0;
	float tileSize = 64.0f;

	Monster* monster;
	std::list<Monster*> monsters;

	bool isCol = false;

	float debugTimer = 0.f;
	float debugDuration = 1.f;
	ObjectPool<Particle> particlePool;
<<<<<<< Updated upstream
=======
	ObjectPool<Particle> fireParticlePool;
	ObjectPool<Particle> portalParticlePoolRed;
	ObjectPool<Particle> portalParticlePoolGreen;
	ObjectPool<Particle> portalParticlePoolPurple;
>>>>>>> Stashed changes
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
	void CreatePortalParticle(int count);
	void CreateFireParticle(int count);
	Monster* CreateMonster(MonsterId id);

	void SpawnDecoGo(int count);
	void SpawnWallDecoGo(int count);
	void SpawnGlowGo();
	void SpawnPortalGlowGo();
	void ModifyWallToRoof();
	void DestroyDecoGo(DecoGo* obj);

	void TilesToIntMap();
	void CalculatorNongroundTiles();
};

