#pragma once
#include "GameObject.h"
#include "Slot.h"

class Slot;

class MenuInventory : public GameObject
{
protected:
	std::map<int, Slot*>slotList;

public:
	MenuInventory(const std::string& n = "");
	virtual ~MenuInventory() override;

	virtual void SetPosition(const sf::Vector2f& p);
	virtual void SetPosition(float x, float y);

	virtual void SetOrigin(Origins origin);
	virtual void SetOrigin(float x, float y);
	virtual void SetOrigin(sf::RectangleShape& rect, Origins origin);

	virtual void Init() override;
	virtual void Release() override;
	virtual void Reset() override;

	virtual void Update(float dt) override;
	virtual void Draw(sf::RenderWindow& window) override;
	

};

