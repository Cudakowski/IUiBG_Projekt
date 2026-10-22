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
    sf::Time dT;
    
    std::array<float, 200> x_data;
    std::array<float, x_data.size()> y_data;

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);
    ImPlot::CreateContext();

    for (int i = 0; i < x_data.size(); i++)
    {
        float x = i - ((float)x_data.size() / 2.0f);
        x_data[i] = x / 20.0f;
    }

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, dT += deltaClock.restart());
        
        for (int i = 0; i < x_data.size(); i++) y_data[i] = sin(x_data[i])+(sin(dT.asSeconds()*cos(x_data[i]+dT.asSeconds())));

        ImGui::Begin("Test window");
        ImPlot::BeginPlot("Wykres");
            ImPlot::SetupAxes("x", "y");
            ImPlot::SetupAxisLimits(ImAxis_X1, -2.0, 2.0);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -2.0, 2.0);
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