#include "stdafx.h"
#include "SceneSkillEditor.h"
#include "Framework.h"
#include "GameObject.h"

SceneSkillEditor::SceneSkillEditor() : Scene(SceneId::SkillEditor)
{
}

void SceneSkillEditor::Init()
{
	Release();

	windowSize = FRAMEWORK.GetWindowSize();

	for (auto go : gameObjects)
	{
		go->Init();
	}
}

void SceneSkillEditor::Release()
{
	for (auto go : gameObjects)
	{
		//go->Release();
		delete go;
	}
}

void SceneSkillEditor::Enter()
{

	Scene::Enter();
}

void SceneSkillEditor::Exit()
{
	Scene::Exit();
}

void SceneSkillEditor::Update(float dt)
{
	Scene::Update(dt);

}

void SceneSkillEditor::Draw(sf::RenderWindow& window)
{
	Scene::Draw(window);
}
