#pragma once
#include "GameObject.h"

class TextGo : public GameObject
{
protected:
public:

	std::string textureId;
	sf::Text text;
	TextGo(const std::string textureId = "", const std::string n = "");
	virtual ~TextGo() override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetString(const std::string& str);
};

