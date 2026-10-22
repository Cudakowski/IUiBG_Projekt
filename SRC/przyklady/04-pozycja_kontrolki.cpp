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
            ImGui::Button("Guzik 1");
            ImVec2 button_top_left = ImGui::GetItemRectMin();
            ImVec2 button_right_bottom = ImGui::GetItemRectMax();
            ImGui::GetForegroundDrawList()->AddRect(button_top_left, button_right_bottom, IM_COL32(255, 0, 0, 255));
            ImGui::Button("Guzik 2");
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}