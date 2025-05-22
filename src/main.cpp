#include "city/new_city.h"
#include "display/display.h"
#include "display/assets.h"

int main()
{
    Assets::loadDirectoryElements();

    City* Berlin = CityFunctions::generateCity("Berlin", 1);
    while (Display::window.isOpen())
    {
        sf::Event event;
        while (Display::window.pollEvent(event))
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(Display::window);
                std::cout << mousePos.x << " " << mousePos.y << std::endl;
            }

            if (event.type == sf::Event::Closed)
            {
                Display::window.close();
            }
            Display::refreshFrame({ Berlin });
        }
    }
}