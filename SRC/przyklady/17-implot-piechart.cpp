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
    
    ImPlotPieChartFlags flags = 0;
    std::array<char*, 4> labels({ "Ziemniaki","Kukurydza","Pszenica","Owies" });
    std::array<float, labels.size()> data ({ 0.20f,  0.30f,  0.12f, 0.05f });

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);
    ImPlot::CreateContext();
 
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        
        ImGui::Begin("Test window");
        ImPlot::BeginPlot("Wykres");
            ImPlot::PlotPieChart(labels.data(), data.data(), (int)labels.size(), 0.5, 0.5, 0.4, "%.2f", 60, flags);
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