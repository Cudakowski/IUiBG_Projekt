#include "imgui.h"
#include "imgui-SFML.h"
#include "implot3d.h"

#include <array>
#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;
    sf::Time dT;
    
    constexpr int N = 20;
    std::array<float, N*N> x;
    std::array<float, N*N> y;
    std::array<float, N*N> z;

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);
    ImPlot3D::CreateContext();

    for (int i = 0; i < N; i++) 
    {
        for (int j = 0; j < N; j++) 
        {
            int idx = i * N + j;
            x[idx] = -1.0f + j * 0.1f;
            y[idx] = -1.0f + i * 0.1f;
        }
    }

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, dT += deltaClock.restart());
        
        for (int i = 0; i < N; i++) 
        {
            for (int j = 0; j < N; j++) 
            {
                int idx = i * N + j;
                z[idx] = sin(2.0f * dT.asSeconds() + sqrt(x[idx] * x[idx] + y[idx] * y[idx]));
            }
        }

        ImGui::Begin("Test window");
        ImPlot3D::BeginPlot("Wykres");
            ImPlot3D::SetupAxesLimits(-1, 1, -1, 1, -1.5, 1.5);
            ImPlot3D::PlotSurface("Fala", x.data(), y.data(), z.data(), N, N, 0.0, 0.0);
        ImPlot3D::EndPlot();
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImPlot3D::DestroyContext();
    ImGui::SFML::Shutdown();
    return 0;
}