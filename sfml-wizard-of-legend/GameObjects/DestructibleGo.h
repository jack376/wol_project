#pragma once
#include "GameObject.h"
class DestructibleGo : public GameObject
{
protected:
	std::string textureId;
	sf::Sprite sprite;
	int durability;

public:
	DestructibleGo(const std::string& textureId = "", const std::string& n = "")
		: GameObject(n), textureId(textureId) {}
	virtual ~DestructibleGo() override {}

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);
	virtual void SetOrigin(sf::RectangleShape& rect, Origins origin);

	void SetFlipX(bool flip);
	void SetFlipY(bool flip);
};

