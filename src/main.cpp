#include "city/new_city.h"
#include "display/display.h"
#include "display/assets.h"

int main()
{
    Assets::loadDirectoryElements();

    int index = 0;
    bool pressed = false;

    Street* underUseStreet = nullptr;
    Display::ConnectionPoint* underUsePoint = nullptr;

    City* Berlin = CityFunctions::generateCity("Berlin", 30); // 800 street limit -> no names left, dont want streets with the same name
    while (Display::window.isOpen())
    {
        sf::Event event;
        while (Display::window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                Display::window.close();
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                underUsePoint = nullptr;
                underUseStreet = nullptr;
                for (const auto& str : Berlin->streets)
                {
                    if (str->shape.getGlobalBounds().contains(sf::Mouse::getPosition(Display::window).x, sf::Mouse::getPosition(Display::window).y))
                    {
                        underUseStreet = str;
                        break;
                    }
                    if (str->frontPoint != nullptr)
                    {
                        if (str->frontPoint->shape.getGlobalBounds().contains(sf::Mouse::getPosition(Display::window).x, sf::Mouse::getPosition(Display::window).y))
                        {
                            underUsePoint = str->frontPoint;
                            break;
                        }
                        
                    }
                    if (str->backPoint != nullptr)
                    {
                        if (str->backPoint->shape.getGlobalBounds().contains(sf::Mouse::getPosition(Display::window).x, sf::Mouse::getPosition(Display::window).y))
                        {
                            underUsePoint = str->backPoint;
                            break;
                        }
                    }
                }
                if (underUseStreet != nullptr)
                {
                    std::cout << "under use streeet name: " << underUseStreet->name << std::endl;
                    std::cout << "back Point: " << underUseStreet->backPoint << std::endl;
                    std::cout << "front Point: " << underUseStreet->frontPoint << std::endl;
                }
                
                if (underUsePoint != nullptr)
                {
                    std::cout << "under use point rotation: " << underUsePoint->shape.getRotation() << std::endl;
                    std::cout << "under use point address: " << underUsePoint << std::endl;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
            {
                pressed = false;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::L) && !pressed)
            {
                index++;
                if (index >= Berlin->streets.size())
                {
                    index = 0;
                }

                std::cout << "name: " << Berlin->streets[index]->name << std::endl;
                sf::Vector2i* pos = new sf::Vector2i();

                pos->x = Berlin->streets[index]->shape.getPosition().x;
                pos->y = Berlin->streets[index]->shape.getPosition().y;

                sf::Mouse::setPosition(*pos, Display::window);

                pressed = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::K) && !pressed)
            {
                index--;
                if (index < 0)
                {
                    index = Berlin->streets.size()-1;
                }

                std::cout << "name: " << Berlin->streets[index]->name << std::endl;
                sf::Vector2i* pos = new sf::Vector2i();

                pos->x = Berlin->streets[index]->shape.getPosition().x;
                pos->y = Berlin->streets[index]->shape.getPosition().y;

                sf::Mouse::setPosition(*pos, Display::window);
                
                pressed = true;
            }

            Display::refreshFrame({ Berlin });
        }
    }
}