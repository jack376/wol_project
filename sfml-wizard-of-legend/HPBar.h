#pragma once
#include "GameObject.h"
class HPBar :
    public GameObject
{
protected:
	sf::Sprite hpBarBG;
	sf::Sprite hpBarFill;
	sf::Sprite hurtBarFill;

	std::string hpBarBGTextureId;
	std::string hpBarFillTextureId;
	std::string hurtBarFillTextureId;

	int* maxHp = nullptr;
	int* hp = nullptr;
	int hurtHp;
public:
	HPBar(const std::string& n = "");
	virtual ~HPBar() override;

	virtual void SetPosition(const sf::Vector2f& p) override;
	virtual void SetPosition(float x, float y) override;

	virtual void SetOrigin(Origins origin) override;
	virtual void SetOrigin(float x, float y) override;

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;

	void SetTarget(int* maxHp, int* hp);
};

