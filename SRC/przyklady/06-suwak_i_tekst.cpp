#include "imgui.h"
#include "imgui-SFML.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;
    float a = 0, xy[2] = {0.0f,0.0f}, rgb[3] = {0.0f,0.0f,0.0f};
    
    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Test window");
            ImGui::SliderFloat("a", &a, 1.0f, 10.0f);
            ImGui::SliderFloat2("x,y", &xy[0], -2.0f, 2.0f);
            ImGui::SliderFloat3("r,g,b", &rgb[0], 0.0f, 1.0f);

            ImGui::Separator();

            ImGui::Text("a=%.3f", a);
            ImGui::Text("xy={%.3f, %.3f}", xy[0],xy[1]);
            ImGui::Text("rgb ={"); ImGui::SameLine();
            ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f },"%.3f", rgb[0]); ImGui::SameLine();
            ImGui::Text(", "); ImGui::SameLine();
            ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "%.3f", rgb[1]); ImGui::SameLine();
            ImGui::Text(", "); ImGui::SameLine();
            ImGui::TextColored({ 0.0f, 0.0f, 1.0f, 1.0f }, "%.3f", rgb[2]); ImGui::SameLine();
            ImGui::Text("}");
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}