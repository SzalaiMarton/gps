#include "display.h"
#include "assets.h"
#include "../custom_exception.h"

namespace Display
{
	int width = 800;
	int height = width * (16 / 9);
	sf::RenderWindow window(sf::VideoMode(width, height), "gps");

	float xRootPosition = width / 2.f, yRootPosition = height /2.f;
}

void Display::displayStreet(Street* street, bool root)
{
	float xPos{}, yPos{};
	if (root)
	{
		xPos = xRootPosition;
		yPos = yRootPosition;
	}
	
	street->shape.setTexture(*Assets::getObjectTexture("street"));
	sf::FloatRect bounds = street->shape.getGlobalBounds();
	street->shape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	street->shape.setPosition(xPos, yPos);
	window.draw(street->shape);
	window.display();
}

void Display::displayCity(City* city)
{
	city->rootStreet->shape.setPosition(xRootPosition, yRootPosition);
	for (auto& str : city->getStreets())
	{
		if (str == city->rootStreet)
		{
			window.draw(str->shape);
			continue;
		}



		window.draw(str->shape);
	}
}

void Display::refreshFrame(const std::vector<City*>& citiesDisplay)
{
	window.clear();

	for (auto& city : citiesDisplay)
	{
		for (auto& str : city->getStreets())
		{
			window.draw(str->shape);
		}
	}

	window.display();
}