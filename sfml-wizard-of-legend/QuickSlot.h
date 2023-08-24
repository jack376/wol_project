#pragma once
#include "GameObject.h"

class Slot;

class QuickSlot : public GameObject
{
protected:
	std::vector<std::string> slotKey;
	std::map<std::string, Slot*>slotList;

public:
	QuickSlot(const std::string& n = "");
	virtual ~QuickSlot() override;

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

	void AllSetActive(bool isActive);

};

