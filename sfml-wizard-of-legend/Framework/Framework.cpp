#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "SceneEditor.h"

Framework::Framework(int w, int h, const std::string& t)
    : screenWidth(w), screenHeight(h), title(t)
{
}

void Framework::Init(int width, int height, const std::string& title)
{
    window.create(sf::VideoMode(width, height), title);

    DATATABLE_MGR.LoadAll();
    // Resource
    RESOURCE_MGR.Init();
    SCENE_MGR.Init();

    ImGui::SFML::Init(window, false);
    ImGuiIO& IO = ImGui::GetIO();
    IO.Fonts->AddFontFromFileTTF("fonts/NanumSquareEB.ttf", 13);
    IO.Fonts->AddFontDefault();
    ImGui::SFML::UpdateFontTexture();
}

void Framework::Release()
{
    SCENE_MGR.Release();
    DATATABLE_MGR.ReleaseAll();
}

void Framework::UpdateEvent(float dt)
{
    SCENE_MGR.UpdateEvent(dt);
}

void Framework::Draw()
{
    SCENE_MGR.Draw(window);
}

void Framework::Run()
{
    Init(screenWidth, screenHeight, title);
    clock.restart();
    sf::Vector2i prevPos = { 0, 0 };

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();
        gamePlayTime += deltaTime;

        INPUT_MGR.Update(dt);

        prevPos = window.getPosition();
        if (prevPos != window.getPosition())
        {
            dt = 0;
        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type)
            {
            case sf::Event::Closed:
                SCENE_MGR.GetCurrScene()->Exit();
                window.close();
                break;
            case sf::Event::GainedFocus:
                break;
            }
            INPUT_MGR.UpdateEvent(event);
        }

        if (window.isOpen())
        {
            UpdateEvent(dt);

            // ImGUI
            ImGui::SFML::Update(window, deltaTime);
            SceneId currentSceneId = SCENE_MGR.GetCurrSceneId();
            switch (currentSceneId)
            {
            case SceneId::Game:

                break;
            case SceneId::Editor:
                Scene* scene = SCENE_MGR.GetCurrScene();
                SceneEditor* sceneEditor = dynamic_cast<SceneEditor*>(scene);
                sceneEditor->DrawEditorUI();
                break;
            }

            window.clear();
            Draw();
            ImGui::SFML::Render(window);
            window.display();
        }
    }
    Release();
    ImGui::SFML::Shutdown();
}

sf::Vector2f Framework::GetWindowSize()
{
    return sf::Vector2f((float)screenWidth, (float)screenHeight);
}

sf::RenderWindow& Framework::GetWindow()
{
    return window;
}

float Framework::GetGamePlayTime()
{
    return gamePlayTime.asSeconds();
}