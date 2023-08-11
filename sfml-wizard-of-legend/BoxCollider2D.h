#pragma once
#include "SpriteGo.h"

class BoxCollider2D : public SpriteGo
{
protected:
	sf::RectangleShape obbCol;

public:
	BoxCollider2D(const std::string& textureId = "", const std::string& n = "");
	virtual ~BoxCollider2D() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	bool ObbCol(const sf::RectangleShape& col1, const sf::RectangleShape& col2);
};

