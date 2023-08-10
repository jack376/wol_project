#pragma once
#include "Singleton.h"

class Framework : public Singleton<Framework>
{
	friend Singleton<Framework>;

protected:
	Framework() = default;
	Framework(int w, int h, const std::string& t);
	virtual ~Framework() override = default;

	sf::RenderWindow window;
	sf::Clock clock;
	sf::Time gamePlayTime = sf::Time::Zero;

	int screenWidth = 1920;
	int screenHeight = 900;
	std::string title = "SFML_Wizard_Of_Legend";

public:
	virtual void Init(int width, int height, const std::string& title);
	virtual void Release();

	virtual void UpdateEvent(float dt);
	virtual void Draw();

	virtual void Run();

	sf::Vector2f GetWindowSize();
	sf::RenderWindow& GetWindow();

	float GetGamePlayTime();
};

#define FRAMEWORK (Framework::Instance())