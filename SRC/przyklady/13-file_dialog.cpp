#include "imgui.h"
#include "imgui-SFML.h"
#include "ImGuiFileDialog.h"

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 640, 480 }), "ImGui + SFML");
    sf::Clock deltaClock;

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);
    
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) window.close();
        }

        ImGui::SFML::Update(window, deltaClock.restart());
        
        ImGui::Begin("Test window");
            if (ImGui::Button("File Dialog")) ImGuiFileDialog::Instance()->OpenDialog("Open", "Open", ".png");
        ImGui::End();

        if (ImGuiFileDialog::Instance()->Display("Open"))
        {
            if (ImGuiFileDialog::Instance()->IsOk()) std::cout << ImGuiFileDialog::Instance()->GetFilePathName() << std::endl;
            ImGuiFileDialog::Instance()->Close();
        }

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}