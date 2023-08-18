#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
Framework::Framework(int w, int h, const std::string& t)
    : screenWidth(w), screenHeight(h), title(t)
{
}

void Framework::Init(int width, int height, const std::string& title)
{
	window.create(sf::VideoMode(width, height), title);
    
    ImGui::SFML::Init(window);
    sf::CircleShape shape(circleRadius, circleSegments);
    shape.setOrigin(circleRadius, circleRadius);
    shape.setPosition(200, 200);

    DATATABLE_MGR.LoadAll();
    // Resource
    RESOURCE_MGR.Init();
    SCENE_MGR.Init();
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

            ImGui::SFML::Update(window, deltaTime);

            // ImGui Run Test Code
            ImGui::Begin("Window title");
            ImGui::Text("Window text!");
            ImGui::Checkbox("Circle", &circleExists);
            ImGui::SliderFloat("Radius", &circleRadius, 100.0f, 300.0f);
            ImGui::SliderInt("Sides", &circleSegments, 3, 150);
            ImGui::InputText("TextBox", buffer, IM_ARRAYSIZE(buffer));
            ImGui::End();

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
