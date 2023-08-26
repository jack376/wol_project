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
class Particle;
class Monster;
class CustomEffect;
class Slot;
class MenuInventory;
class QuickSlot;
enum class MonsterId;

class SceneGame : public Scene
{
protected:
	Player* player;

	MenuInventory* menu;
	QuickSlot* quickSlot;
	Slot* slot1;
	Slot* slot2;

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

	bool isMenuOn = false;
	bool isCol = false;

	float debugTimer = 0.f;
	float debugDuration = 1.f;
	ObjectPool<Particle> particlePool;
	ObjectPool<Particle> fireParticlePool;
	int count = 0;

	sf::View miniMapView;
	sf::RectangleShape miniMapBackground;
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

	Tile* CreateTile(const std::string& name, float posX, float posY, int sort = 1);
	void LoadFromCSV(const std::string& path);
	void CreateTile2dVector(int rows, int cols);
	void CreateParticle(int count);
	Monster* CreateMonster(MonsterId id);

	void SpawnDecoGo(int count);
	void SpawnWallDecoGo(int count);
	void SpawnGlowGoLarge();
	void SpawnGlowGoSmall();

	void DestroyDecoGo(DecoGo* obj);

	void TilesToIntMap();
	void CalculatorNongroundTiles();

	bool GetIsMenuOn() { return isMenuOn; }
	void SetIsMenuOn(bool isOn) { isMenuOn = isOn; }
};

