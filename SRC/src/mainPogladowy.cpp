#include "imgui.h"
#include "imgui-SFML.h"
#include "ImGuiFileDialog.h"


#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

enum Brush {
 NONE, LINE, SQUARE, FILLED_SQUARE, CIRCLE
};

sf::Color toSFMLColor(const ImVec4& c) 
{
    return sf::Color(
        static_cast<std::uint8_t>(c.x * 255.0f),
        static_cast<std::uint8_t>(c.y * 255.0f),
        static_cast<std::uint8_t>(c.z * 255.0f),
        static_cast<std::uint8_t>(c.w * 255.0f)
    );
}

int main() {
    sf::Vector2u windowSize{1800,900};

    sf::RenderWindow window(sf::VideoMode(windowSize), "lab02", sf::Style::Titlebar | sf::Style::Close);
    sf::Clock deltaClock;

    sf::Texture textureBroom{"../Demo/broom.png"};
    sf::Texture textureSquare{"../Demo/square.png"};
    sf::Texture textureWindow{"../Demo/window.png"};
    sf::Texture textureCircle{"../Demo/circle.png"};
    sf::Sprite spriteBroom(textureBroom);
    sf::Sprite spriteSquare(textureSquare);
    sf::Sprite spriteWindow(textureWindow);
    sf::Sprite spriteCircle(textureCircle);

    sf::Vector2u imageSize{1800,900};
    sf::RenderTexture canvas{};
    canvas.resize(imageSize);
    canvas.clear(sf::Color::White);
    auto sprite = sf::Sprite( canvas.getTexture());


    bool isExit{false};
    bool isTools{false};
    bool isAbout{false};

    Brush brush{Brush::NONE};
    ImVec4 colorBorder({ 0.0f, 1.0f, 0.0f, 1.0f }), colorFill({ 0.0f, 1.0f, 0.0f, 1.0f });
    float thickness{1.0f};
    bool isLineGradient{false};

    sf::Vector2f shapeBegin(0.0f, 0.0f);
    sf::Vector2f shapeEnd(0.0f, 0.0f);
    bool isDrawing = {false};


    sf::VertexArray line(sf::PrimitiveType::LineStrip, 2);
    sf::RectangleShape rectangle;
    sf::CircleShape circle;

    window.setFramerateLimit(60);
    std::ignore = ImGui::SFML::Init(window);

    

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>()) isExit = true;
            if(!ImGui::GetIO().WantCaptureMouse)
            {
                if (auto m = event->getIf<sf::Event::MouseButtonPressed>())
                {
                    isDrawing = true ; 
                    shapeBegin =  {m->position.x, m->position.y};
                } 
                if (auto m = event->getIf<sf::Event::MouseButtonReleased>()) 
                {
                    isDrawing = false;
                    //shapeEnd =  {m->position.x, m->position.y};


                    switch (brush)
                    {
                    case Brush::LINE:
                        // line[0].position = shapeBegin;     line[0].color = toSFMLColor(colorBorder);
                        // if(isLineGradient)
                        // {
                        //     line[1].position = shapeEnd; line[1].color = toSFMLColor(colorFill);
                        // } else {
                        //     line[1].position = shapeEnd; line[1].color = toSFMLColor(colorBorder);
                        // }
                        canvas.draw(line);

                        break;
                    case Brush::SQUARE:
                        /* code */
                        canvas.draw(rectangle);
                        break;
                    case Brush::FILLED_SQUARE:
                        /* code */
                        canvas.draw(rectangle);
                        break;
                    case Brush::CIRCLE:
                        /* code */
                        canvas.draw(circle);
                        break;
                    default:
                        break;
                    }
                }
                if (auto m = event->getIf<sf::Event::MouseMoved>())
                {
                    shapeEnd =  {m->position.x, m->position.y};
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) 
                { 
                    canvas.clear();
                    // = sf::Texture(sf::Image(sf::Vector2u{1800,900}, sf::Color::White));  
                    imageSize = {1800,900};
                    canvas.resize(imageSize);
                    canvas.clear(sf::Color::White);
                } // czyści całą zawartość ekranu
                ImGui::Separator();
                if (ImGui::MenuItem("Open")) { ImGuiFileDialog::Instance()->OpenDialog("Open", "Open", ".png"); }// otwiera okno dialogowe pozwalające wybrać nazwę pliku PNG, następnie plik ten jest wczytywany – zakładamy, że ma on rozmiar 1800pikseli x 900pikseli
                ImGui::Separator();
                if (ImGui::MenuItem("Save As...")) { ImGuiFileDialog::Instance()->OpenDialog("Save", "Save As...", ".png"); } //otwiera okno dialogowe, gdzie użytkownik może wybrać nazwę pliku PNG, następnie cała zawartość okna (bez interfejsu użytkownika!!) jest zapisywana do wskazanego pliku
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) { isExit = true;} // powoduje pojawienie się okna z pytaniem o potwierdzenie i jeśli użytkownik potwierdzi – kończymy program
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            { 
                if (ImGui::Checkbox("Show Tool Options", &isTools)) {  } // jest klasycznym „checkbox’em”, jeśli jest on zaznaczony to w okienku „Tools” pojawiają się dodatkowe opcje w zależności od wybranego narzędzia rysowania
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("About")) { isAbout = true;  } // wybranie tej opcji powoduje, że wyskakuje okienko z informacją o twórcy programu i przyciskiem „OK” zamykającym to okno
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (ImGuiFileDialog::Instance()->Display("Open"))
        {
            if (ImGuiFileDialog::Instance()->IsOk()) 
            {
                sf::Texture loadedTexture;
                if(loadedTexture.loadFromFile(ImGuiFileDialog::Instance()->GetFilePathName()) )
                {
                    imageSize = loadedTexture.getSize();
                    canvas.resize(imageSize);
                    canvas.clear(sf::Color::White);
                    canvas.draw(sf::Sprite(loadedTexture));
                }
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGuiFileDialog::Instance()->Display("Save"))
        {
            if (ImGuiFileDialog::Instance()->IsOk()) 
            {
                canvas.getTexture().copyToImage().saveToFile(ImGuiFileDialog::Instance()->GetFilePathName());
            }
            ImGuiFileDialog::Instance()->Close();
        }

        if(isExit) 
        {
            ImGui::Begin("Exit");
                ImGui::Text("Jestes pewny czy chcesz wyjsc?");
                if(ImGui::Button("Anuluj")) isExit = false;
                if(ImGui::Button("Wyjdz")) window.close();
            ImGui::End();
        }

        ImGui::Begin("Tools");
            if(ImGui::ImageButton("Broom", spriteBroom, { 50,50 }, { 255,255,255 })) brush = Brush::LINE;
            ImGui::SameLine();
            if(ImGui::ImageButton("Square", spriteSquare, { 50,50 }, { 255,255,255 })) brush = Brush::SQUARE;
            if(ImGui::ImageButton("Filled Square", spriteWindow, { 50,50 }, { 255,255,255 })) brush = Brush::FILLED_SQUARE;
            ImGui::SameLine();
            if(ImGui::ImageButton("Circle", spriteCircle, { 50,50 }, { 255,255,255 })) brush = Brush::CIRCLE;

            ImGui::Separator();

            ImGui::Text("Border COLOR");
            ImGui::ColorEdit3("BorderColorEdit", (float*)&colorBorder, ImGuiColorEditFlags_NoLabel);
            ImGui::Text("Fill COLOR");
            ImGui::ColorEdit3("FillColorEdit", (float*)&colorFill, ImGuiColorEditFlags_NoLabel);



            if(isTools) 
            {
                ImGui::Separator();

                switch (brush)
                {
                case Brush::LINE:
                    ImGui::Checkbox("Gradient", &isLineGradient);
                    break;
                case Brush::SQUARE:
                case Brush::FILLED_SQUARE:
                case Brush::CIRCLE:
                    ImGui::SliderFloat("Thickness", &thickness, 1.0f, 10.0f);
                    break;
                
                default:
                    break;
                }
            }
        ImGui::End();


        //sf::RenderTexture d{};
        //d.draw();

        if(isAbout) 
        {
            ImGui::Begin("About");
                ImGui::Text("Autor: Wojciech Osowski");
                if(ImGui::Button("OK")) isAbout = false;
            ImGui::End();
        }

        //sprite.setPosition({(static_cast<float>(windowSize.x)-imageSize.x)/2.0f, (static_cast<float>(windowSize.y)-imageSize.y)/2.0f});


        window.clear(sf::Color(40,40,40));
        canvas.display();
        window.draw(sprite);

        if(isDrawing)
        {
            switch (brush)
            {
            case Brush::LINE:
                line[0].position = shapeBegin;     line[0].color = toSFMLColor(colorBorder);
                if(isLineGradient)
                {
                    line[1].position = shapeEnd; line[1].color = toSFMLColor(colorFill);
                } else {
                    line[1].position = shapeEnd; line[1].color = toSFMLColor(colorBorder);
                }
                window.draw(line);
                break;
            case Brush::SQUARE:
                rectangle.setSize(sf::Vector2f{std::abs(shapeBegin.x-shapeEnd.x), std::abs(shapeBegin.y-shapeEnd.y)});
                rectangle.setPosition(sf::Vector2f(std::min(shapeBegin.x,shapeEnd.x), std::min(shapeBegin.y,shapeEnd.y)));
                rectangle.setFillColor(sf::Color::Transparent);
                rectangle.setOutlineColor(toSFMLColor(colorBorder));
                rectangle.setOutlineThickness(thickness);
                window.draw(rectangle);
                break;
            case Brush::FILLED_SQUARE:
                rectangle.setSize(sf::Vector2f{std::abs(shapeBegin.x-shapeEnd.x), std::abs(shapeBegin.y-shapeEnd.y)});
                rectangle.setPosition(sf::Vector2f(std::min(shapeBegin.x,shapeEnd.x), std::min(shapeBegin.y,shapeEnd.y)));
                rectangle.setFillColor(toSFMLColor(colorFill));
                rectangle.setOutlineColor(toSFMLColor(colorBorder));
                rectangle.setOutlineThickness(thickness);
                window.draw(rectangle);
                break;
            case Brush::CIRCLE:
            {

                circle.setPosition(shapeBegin);
                float r = (shapeBegin-shapeEnd).length();
                circle.setOrigin({r,r});
                circle.setRadius((shapeBegin-shapeEnd).length());
                circle.setFillColor(sf::Color::Transparent);
                circle.setOutlineColor(toSFMLColor(colorBorder));
                circle.setOutlineThickness(thickness);
                window.draw(circle);
                break;
            }
            default:
                break;
            }
        }


        ImGui::SFML::Render(window);
        window.display();
    }
    ImGui::SFML::Shutdown();
    return 0;
}