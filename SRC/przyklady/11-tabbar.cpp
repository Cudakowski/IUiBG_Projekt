#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        
        ImGui::Begin("Test window");
        if (ImGui::BeginTabBar("TabBar"))
        {
            if (ImGui::BeginTabItem("Colors"))
            {
                ImGui::BulletText("Red");
                ImGui::BulletText("Green");
                ImGui::BulletText("Blue");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Sizes"))
            {
                ImGui::BulletText("Small");
                ImGui::BulletText("Medium");
                ImGui::BulletText("Big");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Speeds"))
            {
                ImGui::BulletText("Slow");
                ImGui::BulletText("Fast");
                ImGui::BulletText("Ludicrous Speed");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}