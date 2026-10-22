#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"

#include <array>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;
    
    std::array<float, 1000> x_data;
    std::array<float, x_data.size()> y_data;

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);
    ImPlot::CreateContext();

    for (int x = 0; x < x_data.size(); x++)
    {
        x_data[x] = (float)x / 100.0f;
        y_data[x] = sin(x / 100.0f);
    }
    
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        
        ImGui::Begin("Test window");
        ImPlot::BeginPlot("Wykres");
            ImPlot::PlotLine("sin(x)", x_data.data(), y_data.data(), (int)x_data.size());
        ImPlot::EndPlot();
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();
    return 0;
}