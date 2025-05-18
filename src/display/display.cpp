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

sf::Vector2f Display::getFrontOfShape(sf::Sprite& shape)
{
	sf::FloatRect bounds = shape.getGlobalBounds();
	
	// 1 - psitive addition, 0 - no addition, -1 - negative addition
	short xAdditionTransformer{}, yAdditionTransformer{}; 

	// get rotation and calculating how the postion should be calculated
	float rotation = shape.getRotation();
	short fullRotationCount = rotation / 360;
	rotation -= 360 * fullRotationCount;
	
	if (rotation == 0)
	{
		xAdditionTransformer = 1;
		yAdditionTransformer = 0;
	}
	else if (rotation == 90)
	{
		xAdditionTransformer = 0;
		yAdditionTransformer = 1;
	}
	else if (rotation == 180)
	{
		xAdditionTransformer = -1;
		yAdditionTransformer = 0;
	}
	else if (rotation == 270)
	{
		xAdditionTransformer = 0;
		yAdditionTransformer = -1;
	}

	return *new sf::Vector2f(bounds.left + (bounds.width*xAdditionTransformer), bounds.top + (bounds.height * yAdditionTransformer));
}
