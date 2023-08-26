#pragma once
#include "GameObject.h"

class Player;
class GlowGo : public GameObject
{
protected:

	std::string textureId;
	sf::Sprite sprite;

	Player* player = nullptr;

public:
	GlowGo(const std::string& textureId = "", const std::string& n = "")
		: GameObject(n), textureId(textureId) {}
	virtual ~GlowGo() override {}

	virtual void Init() override {}
	virtual void Release() override {}

	virtual void Reset() override;
	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetPosition(const sf::Vector2f& p);
	void SetPosition(float x, float y);
	void SetOrigin(Origins origin);
	void SetOrigin(float x, float y);
	void SetTexture(const std::string& id);
	void SetScale(float x, float y);

	void SetPlayer(Player* player) { this->player = player; }
};

