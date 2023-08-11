#pragma once
#include "Scene.h"

class UIButton;
class TextGo;
class SpriteGo;
class Tile;

class Player;
class ElementalSpell;
class Monster;
enum class MonsterId;

class SceneGame : public Scene
{
protected:
	Player* player;
	ElementalSpell* tempWindSlash;
	std::vector<std::vector<Tile*>> tilesWorld;

public:
	SceneGame();
	virtual ~SceneGame() override = default;

	virtual void Init() override;
	virtual void Release() override;

	virtual void Enter() override;
	virtual void Exit() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	Player* GetPlayer() { return player; }
	Tile* CreateTile(const std::string& name, float posX, float posY, int sort = 0);
	void LoadFromCSV(const std::string& path);
	Monster* CreatMonster(MonsterId id);
};

