#include "city/new_city.h"
#include <iostream>
#include "display/assets.h"
#include "display/display.h"

int main()
{
    bool isDragging = false;
    sf::Vector2i lastMousePos{};

    Assets::loadDirectoryElements();
    
    City* Berlin = CityFunctions::generateCity("Berlin", 20); // 800 point limit -> no names left, dont want points with the same name

    Display::displayCity(Berlin);

    std::cout << "\npoints count: " << Berlin->points.size() << " streets count: " << Berlin->streets.size() << std::endl;
    Berlin->countDisplayedPoints();
    Berlin->countMoreThan2ConnectionStreets();
    //Berlin->printPoints(true, true);
    //Berlin->printStreets(true);

    while (Display::window.isOpen())
    {
        sf::Event event;
        while (Display::window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                Display::window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                isDragging = true;
                lastMousePos = sf::Mouse::getPosition(Display::window);

                sf::Vector2f mouseWorldPos = Display::window.mapPixelToCoords(lastMousePos);
                Object* obj = Display::getObjectByMouse((sf::Vector2i)mouseWorldPos, Berlin);
                Street* currentStr = dynamic_cast<Street*>(obj);
                ConnectionPoint* currentPoint = dynamic_cast<ConnectionPoint*>(obj);
                if (currentPoint != nullptr)
                {
                    std::cout << "current point: " << currentPoint->name << std::endl;
                    std::cout << "maxConnections: " << (int)currentPoint->maxConnection << "\n" << std::endl;
                }
                if (currentStr != nullptr)
                {
                    std::cout << "current str: " << currentStr->name << std::endl;

                    if (currentStr->backPoint)
                        std::cout << "backName: " << currentStr->backPoint->name << std::endl;
                    if (currentStr->frontPoint)
                        std::cout << "frontPoint: " << currentStr->frontPoint->name << std::endl;
                    std::cout << std::endl;
                }
            }
            if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            {
                isDragging = false;
            }

            if (event.type == sf::Event::MouseWheelScrolled) 
            {
                if (event.mouseWheelScroll.delta > 0)
                {
                    Display::camera.zoom(0.9f);
                    Display::window.setView(Display::camera);
                }
                else
                {
                    Display::camera.zoom(1.1f);
                    Display::window.setView(Display::camera);
                }
            }
        }

        if (isDragging)
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(Display::window);
            sf::Vector2f delta = Display::window.mapPixelToCoords(lastMousePos) - Display::window.mapPixelToCoords(mousePos);
            Display::camera.move(delta);
            lastMousePos = mousePos;
            Display::window.setView(Display::camera);
        }

        Display::refreshFrame({ Berlin });
    }

    delete Berlin;
    for (auto texture : Assets::textureVector)
    {
        delete texture;
    }
    Assets::textureVector.clear();
}