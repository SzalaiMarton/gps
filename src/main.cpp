#include "city/new_city.h"
#include <iostream>
//#include "display/display.h"
//#include "display/assets.h"

int main()
{
    //Assets::loadDirectoryElements();
    
    City* Berlin = CityFunctions::generateCity("Berlin", 7); // 800 street limit -> no names left, dont want streets with the same name

    while (true)
    {
        std::string from;
        std::string to;

        std::cout << "Get the shortest route between: " << std::endl;
        std::cin >> from;
        std::cin >> to;

        Berlin->getShortestRoute(from, to, Berlin);
    }

    delete Berlin;

    /*while (Display::window.isOpen())
    {
        sf::Event event;
        while (Display::window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                Display::window.close();
            }
            Display::refreshFrame({ Berlin });
        }
    }*/
}