#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;
    sf::Texture texture_button("edit.png");
    sf::Sprite sprite_button(texture_button);
    
    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Test window");
           ImGui::ImageButton("test1", sprite_button, { 50,50 }, { 255,255,255 });
           ImGui::ImageButton("test2", sprite_button, { 75,75 }, { 255,0,0 });
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}