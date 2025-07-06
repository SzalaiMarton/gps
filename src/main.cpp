#include "city/new_city.h"
#include <iostream>
#include "display/assets.h"
#include "display/display.h"
#include <chrono>


int main()
{
    ConnectionPoint* basePoint{}, *destinationPoint{};
    bool isDragging = false;
    sf::Vector2i lastMousePos{};

    LOG("Loading assets.");
    Assets::loadDirectoryElements();

    auto start = std::chrono::high_resolution_clock::now();
    LOG("Generating new city.");
    City* Berlin = CityFunctions::generateCity("Berlin", 800); // 800 point limit -> no names left, dont want points with the same name
    LOG("Finished generating new city.");
    
    LOG("Displaying " << Berlin->name);
    Display::displayCity(Berlin);
    LOG("Displaying " << Berlin->name << " finished.");

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    LOG("Time elapsed: " << elapsed.count() << "s.");
    LOG("\npoints count: " << Berlin->points.size() << " streets count: " << Berlin->streets.size());
    
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
                        basePoint = currentPoint;
                        FORCE_LOG("Base point: " << currentPoint->name);
                    }
                    else if (destinationPoint == nullptr)
                    {
                        destinationPoint = currentPoint;
                        FORCE_LOG("Destination point: " << currentPoint->name << "\n");
                        Display::displayRoute(Berlin->getShortestPath(basePoint, destinationPoint));
                    }
                    else
                    {
                        FORCE_LOG("Current point: " << currentPoint->name);
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
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                basePoint = nullptr;
                destinationPoint = nullptr;
                CityFunctions::clearRoute();
                FORCE_LOG("Base and destination points cleared.");
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
                    Display::refreshFrame({ Berlin });
                }
                else
                {
                    Display::camera.zoom(1.2f);
                    Display::window.setView(Display::camera);
                    Display::refreshFrame({ Berlin });
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
            Display::refreshFrame({ Berlin });
        }

        Display::refreshFrame({ Berlin });
    }

    delete Berlin;
    for (auto texture : Assets::textureVector)
    {
        delete texture;
    }
    for (auto str : CityFunctions::route)
    {
        delete str;
    }
    CityFunctions::route.clear();
    Assets::textureVector.clear();
}