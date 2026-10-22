#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;
    ImVec4 color1({ 0.0f, 1.0f, 0.0f, 1.0f }), color2({ 0.0f, 1.0f, 0.0f, 1.0f });

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Test window");
            ImGui::ColorEdit3("ColorEdit", (float*)&color1, ImGuiColorEditFlags_NoLabel);
            ImGui::Separator();
            ImGui::ColorPicker4("ColorPicker", (float*)&color2);
            ImGui::Separator();
            ImGui::ColorButton("ColorButton", color2);
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}