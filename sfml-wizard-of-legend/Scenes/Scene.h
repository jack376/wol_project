#pragma once

class GameObject;
class TextGo;

class Scene
{
protected:
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	SceneId sceneId;

	bool isPlaying = true;

	//std::vector<std::tuple<ResourceTypes, std::string>> resources;


	std::list<GameObject*> gameObjects; 
	std::list<GameObject*> removeGameObjects;
	std::list<GameObject*> notPauseObjects;

	sf::View worldView;
	sf::View uiView;

	sf::RenderWindow& window;

	float blinkTimer = 0.f;
	float blinkDuration = 0.5f;

public:

	Scene(SceneId id = SceneId::None);
	virtual ~Scene();

	std::string resourceListPath;

	GameObject* FindGo(const std::string& name);
	void FindGos(std::list<GameObject*>& list, const std::string& name);
	bool Exist(GameObject* go);
	bool ExistNP(GameObject* go);
	GameObject* AddGo(GameObject* go);
	GameObject* AddNPGo(GameObject* go);
	void RemoveGo(GameObject* go);
	void SortGos();

	sf::Vector2f ScreenToWorldPos(sf::Vector2f screenPos);
	sf::Vector2f ScreenToUiPos(sf::Vector2f screenPos);
	sf::Vector2f WorldPosToScreen(sf::Vector2f worldPos);
	sf::Vector2f UiPosPosToScreen(sf::Vector2f uiPos);
	sf::Vector2f UiPosToWorldPos(sf::Vector2f uiPos);
	sf::Vector2f WorldPosToUiPos(sf::Vector2f worldPos);

	virtual void Init() = 0;
	virtual void Release() = 0;

	virtual void Enter();
	virtual void Exit();

	virtual void Update(float dt);
	virtual void Draw(sf::RenderWindow& window);

	void Blink(TextGo* go);

	void SetIsPlaying(bool isPlaying) { this->isPlaying = isPlaying; }
};

