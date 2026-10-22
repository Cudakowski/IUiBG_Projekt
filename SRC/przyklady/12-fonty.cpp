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
    
    ImGui::GetIO().Fonts->AddFontDefault();
    ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF("Roboto-Bold.ttf", 22.0f);
    std::ignore = ImGui::SFML::UpdateFontTexture();
    IM_ASSERT(font != NULL);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        
        ImGui::Begin("Test window");
            ImGui::PushFont(font);
                ImGui::Text("To jest font Roboto (o rozmarze 22pt)");
            ImGui::PopFont();
            ImGui::Text("To jest domyœlny font.");
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}