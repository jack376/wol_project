#pragma once
#include "Scene.h"

class TextGo;
class SpriteGo;
class BaseUI;
class Tile;

class SceneEditor : public Scene
{
protected:
	std::vector<Tile*> tiles;

	sf::Sprite camera;
	sf::Vector2f cameraDirection;
	sf::Vector2f cameraPosition;
	float cameraSpeed = 1000.0f;

	float refreshDelay = 1.0f;

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
};