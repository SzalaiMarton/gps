#include "city/new_city.h"
#include "display/display.h"
#include "display/assets.h"

int main()
{
    Assets::loadDirectoryElements();

    City* Berlin = CityFunctions::generateCity("Berlin", 30);
    while (Display::window.isOpen())
    {
        sf::Event event;
        while (Display::window.pollEvent(event))
        {


            if (event.type == sf::Event::Closed)
            {
                Display::window.close();
            }
            //Display::refreshFrame({Berlin});
        }
    }
}