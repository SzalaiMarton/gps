#include "city/new_city.h"
#include <iostream>
//#include "display/display.h"
//#include "display/assets.h"

int main()
{
    //Assets::loadDirectoryElements();
    
    City* Berlin = CityFunctions::generateCity("Berlin", 25); // 800 street limit -> no names left, dont want streets with the same name

    Berlin->printPoints(false);
    //Berlin->printStreets(true);

    while (true)
    {
        std::string from;
        std::string to;

        std::cout << "Get the shortest route between: \n(from)\n(to)" << std::endl;
        //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, from);
        std::getline(std::cin, to);

        std::vector<ConnectionPoint*> path = Berlin->getShortestPath(from, to);
        
        for (int i = 0; i < path.size(); i++)
        {
            if (i == path.size() - 1)
                std::cout << path[i]->name;
            else
                std::cout << path[i]->name << "->";
        }
        std::cout << std::endl;
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