#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"

#include <array>
#include <random>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;
    
    std::array<float, 20> x_data;
    std::array<float, x_data.size()> y_data;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(-10.0, 10.0);

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);
    ImPlot::CreateContext();

    for (int x = 0; x < x_data.size(); x++)
    {
        x_data[x] = dist(mt);
        y_data[x] = dist(mt);
    }
    
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        
        ImGui::Begin("Test window");
        ImPlot::BeginPlot("Wykres");
            ImPlot::SetupAxes("x", "y");
            ImPlot::SetupAxisLimits(ImAxis_X1, -10.0, 10.0);
            ImPlot::SetupAxisLimits(ImAxis_Y1, -10.0, 10.0);
            ImPlot::PlotScatter("Punkty", x_data.data(), y_data.data(), (int)x_data.size());
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