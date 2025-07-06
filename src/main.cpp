#include "city/new_city.h"
#include <iostream>
#include "display/assets.h"
#include "display/display.h"
#include <chrono>

#define FORCE_LOG(s) std::cout << s << std::endl

int main()
{
    ConnectionPoint* basePoint{}, *destinationPoint{};
    bool isDragging = false;
    sf::Vector2i lastMousePos{};
    

    LOG("Loading assets.");
    Assets::loadDirectoryElements();

    auto start = std::chrono::high_resolution_clock::now();
    LOG("Generating new city.");
    City* Berlin = CityFunctions::generateCity("Berlin", 500); // 800 point limit -> no names left, dont want points with the same name
    LOG("Finished generating new city.");
    
    LOG("Displaying " << Berlin->name);
    Display::displayCity(Berlin);
    LOG("Displaying " << Berlin->name << " finished.");

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    LOG("Time elapsed: " << elapsed.count() << "s.");
    LOG("\npoints count: " << Berlin->points.size() << " streets count: " << Berlin->streets.size());

    Berlin->countDisplayedPoints();
    //Berlin->countMoreThan2ConnectionStreets();
    //Berlin->printPoints(false, true);
    //Berlin->printStreets(true);
    
    Display::window.setFramerateLimit(60);

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
                ConnectionPoint* currentPoint = dynamic_cast<ConnectionPoint*>(Display::getObjectByMouse((sf::Vector2i)mouseWorldPos, Berlin));
                if (currentPoint != nullptr)
                {
                    if (basePoint == nullptr)
                    {
                        FORCE_LOG("Base point: " << currentPoint->name);
                        FORCE_LOG("Pick another point to get shortest route.");
                    }
                    else if (destinationPoint == nullptr)
                    {
                        FORCE_LOG("Destination point: " << currentPoint->name);
                        Display::displayRoute(Berlin->getShortestPath(basePoint, destinationPoint));
                    }

                    DEBUG_LOG("maxConnections: " << (int)currentPoint->maxConnection);
                    DEBUG_LOG("Connected points: ");
                    for (auto& point : currentPoint->connectedPoints)
                    {
                        DEBUG_LOG("\t" << point->name);
                    }
                    DEBUG_LOG("\n");
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
                    Display::camera.zoom(0.8f);
                    Display::window.setView(Display::camera);
                }
                else
                {
                    Display::camera.zoom(1.2f);
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