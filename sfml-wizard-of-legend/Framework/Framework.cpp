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

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("fonts/NanumSquareEB.ttf", 13, nullptr, io.Fonts->GetGlyphRangesKorean());
    io.Fonts->AddFontDefault();
    ImGui::SFML::UpdateFontTexture();

    avgFps = 0.0f;
    minFps = std::numeric_limits<float>::max();
    maxFps = std::numeric_limits<float>::min();
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

        // FPS Monitor
        fps = 1.0f / dt;
        frameCount++;
        elapsedTime += dt;
        if (elapsedTime >= 1.0f)
        { 
            avgFps = frameCount / elapsedTime;
            elapsedTime = 0.0f;
            frameCount = 0;

            if (fps < minFps) { minFps = fps; }
            if (fps > maxFps) { maxFps = fps; }
        }

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

            if (INPUT_MGR.GetKeyDown(sf::Keyboard::F12))
            {
                showFps = !showFps;
            }
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
                if (!ImGui::IsAnyItemActive() && !ImGui::IsWindowFocused())
                {
                    sceneEditor->InputEditorUI();
                    sceneEditor->CemeraEditorUI(dt);
                }
                break;
            }

            // FPS Monitor
            if (showFps)
            {
                ImGui::SetNextWindowPos(ImVec2(8.0f, 8.0f));
                ImGui::SetNextWindowSize(ImVec2(192.0f, 96.0f));
                ImGui::Begin("FPS Monitor", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground);
                {
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "FPS: %.2f", fps);
                    ImGui::Text("AVG FPS: %.2f", avgFps);
                    ImGui::Text("MIN FPS: %.2f", minFps);
                    ImGui::Text("MAX FPS: %.2f", maxFps);
                }
                ImGui::End();
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