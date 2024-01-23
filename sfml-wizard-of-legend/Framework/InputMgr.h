#pragma once
#include "stdafx.h"
#include "Singleton.h"

enum class Axis
{
	Horizontal,
	Vertical,
};

enum class SOCDType
{
	Neutrality,
	After,
};

struct AxisInfo
{
	Axis axis;
	std::list<int> positivies;
	std::list<int> negatives;
	
	float sensi = 3.f;
	float value = 0.f;
};


class InputMgr : public Singleton<InputMgr>
{
	friend Singleton<InputMgr>;
	sf::Event inputEv;
protected:
	InputMgr();
	virtual ~InputMgr() override = default;

	std::list<int> downList;
	std::list<int> upList;
	std::list<int> ingList;

	sf::Vector2f mousePos;

	std::map<Axis, AxisInfo> axisInfoMap;

	std::vector<char> textList;
public:
	void Update(float dt);
	void UpdateEvent(const sf::Event& ev);

	sf::Event& GetEvent() { return inputEv; }

	// Keyboard
	bool GetKeyDown(sf::Keyboard::Key key);
	bool GetKey(sf::Keyboard::Key key);
	bool GetKeyUp(sf::Keyboard::Key key);

	// Mouse
	const sf::Vector2f& GetMousePos();
	bool GetMouseButtonDown(sf::Mouse::Button button);
	bool GetMouseButton(sf::Mouse::Button button);
	bool GetMouseButtonUp(sf::Mouse::Button button);


	// Axis
	float GetAxis(Axis axis);
	float GetAxisRaw(Axis axis, SOCDType type);

	std::vector<char>& GetTextList() { return textList;};
};

#define INPUT_MGR (InputMgr::Instance())
