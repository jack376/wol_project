#include "stdafx.h"
#include "Scene.h"
#include "GameObject.h"
#include "ResourceMgr.h"
#include "Framework.h"
#include "TextGo.h"

Scene::Scene(SceneId id) : sceneId(id), window(FRAMEWORK.GetWindow())
{

}

Scene::~Scene()
{
}

GameObject* Scene::FindGo(const std::string& name)
{
	//auto compare = [name](GameObject* go) { return go->GetName() == name; };
	//auto it = std::find_if(gameObjects.begin(), gameObjects.end(), compare);
	//if (it == gameObjects.end())
	//	return nullptr;
	//return *it;

	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			return go;
		}
	}
	return nullptr;
}

void Scene::FindGos(std::list<GameObject*>& list, const std::string& name)
{
	//list.clear();
	for (auto go : gameObjects)
	{
		if (go->GetName() == name)
		{
			list.push_back(go);
		}
	}
}

bool Scene::Exist(GameObject* go)
{
	return std::find(gameObjects.begin(), gameObjects.end(), go) != gameObjects.end();
}

bool Scene::ExistNP(GameObject* go)
{
	return std::find(notPauseObjects.begin(), notPauseObjects.end(), go) != notPauseObjects.end();
}

GameObject* Scene::AddGo(GameObject* go)
{
	if (!Exist(go))
	{
		gameObjects.push_back(go);
	}
	return go;
}

GameObject* Scene::AddNPGo(GameObject* go)
{
	if (!ExistNP(go))
	{
		notPauseObjects.push_back(go);
	}
	return go;
}

void Scene::RemoveGo(GameObject* go)
{
	removeGameObjects.push_back(go);
}

void Scene::SortGos()
{
	gameObjects.sort([](GameObject* lhs, GameObject* rhs) {
		if (lhs->sortLayer != rhs->sortLayer)
			return lhs->sortLayer < rhs->sortLayer;
		return lhs->sortOrder < rhs->sortOrder;
		});
}

sf::Vector2f Scene::ScreenToWorldPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, worldView);
}

sf::Vector2f Scene::ScreenToUiPos(sf::Vector2f screenPos)
{
	return window.mapPixelToCoords((sf::Vector2i)screenPos, uiView);
}

sf::Vector2f Scene::WorldPosToScreen(sf::Vector2f worldPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(worldPos, worldView);
}

sf::Vector2f Scene::UiPosPosToScreen(sf::Vector2f uiPos)
{
	return (sf::Vector2f)window.mapCoordsToPixel(uiPos, uiView);
}

sf::Vector2f Scene::UiPosToWorldPos(sf::Vector2f uiPos)
{
	return ScreenToWorldPos(UiPosPosToScreen(uiPos));
}

sf::Vector2f Scene::WorldPosToUiPos(sf::Vector2f worldPos)
{
	return ScreenToUiPos(WorldPosToScreen(worldPos));
}

void Scene::Enter()
{
	//scripts/[SceneId]ResourceList.csv
	RESOURCE_MGR.LoadFromCSV(resourceListPath);

	for (auto go : gameObjects)
	{
		go->Reset();
	}

	blinkTimer = 0.f;
}

void Scene::Exit()
{
	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();

	RESOURCE_MGR.UnLoadAll();

	blinkTimer = 0.f;
}

void Scene::Update(float dt)
{
	if (isPlaying)
	{
		for (auto go : gameObjects)
		{
			if (go->GetActive())
			{
				go->Update(dt);
			}
		}
	}

	if (!isPlaying)
	{
		for (auto go : notPauseObjects)
		{
			if (go->GetActive())
			{
				go->Update(dt);
			}
		}
	}

	for (auto go : removeGameObjects)
	{
		gameObjects.remove(go);
	}
	removeGameObjects.clear();

	blinkTimer += dt;

}

void Scene::Draw(sf::RenderWindow& window)
{
	// layer 100 บฮลอดย ui Layer

	SortGos();

	window.setView(worldView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer >= 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}

	window.setView(uiView);
	for (auto go : gameObjects)
	{
		if (go->sortLayer < 100)
			continue;

		if (go->GetActive())
		{
			go->Draw(window);
		}
	}
}

void Scene::Blink(TextGo* go)
{
	if (blinkTimer > blinkDuration && blinkTimer < blinkDuration * 2.0f)
	{
		go->SetActive(true);
	}
	if (blinkTimer < blinkDuration && blinkTimer > 0)
	{
		go->SetActive(false);
	}

	if (blinkTimer > blinkDuration * 2.0f)
		blinkTimer = 0.f;
}
