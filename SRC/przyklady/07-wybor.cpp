#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;
    bool abs = true;
    int v=0;
    std::string kolor[]{ "niebieski","zielony","czerwony" };

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Test window");
            ImGui::Checkbox("ABS", &abs);
            ImGui::RadioButton(kolor[0].c_str(), &v, 0);
            ImGui::RadioButton(kolor[1].c_str(), &v, 1);
            ImGui::RadioButton(kolor[2].c_str(), &v, 2);

            ImGui::Separator();

            if (!abs) ImGui::Text("Oh no!!!!");
            ImGui::Text("%s",kolor[v]);
            
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}