#pragma once

#include "../city/new_city.h"

namespace Display
{
	extern int width;
	extern int height;
	extern sf::RenderWindow window;

	extern float xRootPosition, yRootPosition;
	
	void displayStreet(Street* street, bool root = false);
	void displayCity(City* city);
	void refreshFrame(const std::vector<City*>& citiesDisplay);
	sf::Vector2f getFrontOfShape(sf::Sprite& shape);
}
