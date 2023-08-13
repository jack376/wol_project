#pragma once
#include "SpriteGo.h"

class BoxCollider2D : public SpriteGo
{
protected:
	sf::RectangleShape obbCol;

	sf::Sprite sprite1;
public:
	BoxCollider2D(const std::string& textureId = "", const std::string& n = "");
	virtual ~BoxCollider2D() override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	sf::RectangleShape& GetObbCol() { return obbCol; }

	void SetSprite(sf::Sprite& sprite) { this->sprite1 = sprite; }
	void SetColSize();
	bool ObbCol(const sf::RectangleShape& col1, const sf::RectangleShape& col2);
};

