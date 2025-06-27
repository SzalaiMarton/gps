#pragma once

#include "../../include/SFML/Graphics.hpp"
#include "../city/new_city.h"

namespace Display
{
	enum DisplaySide
	{
		FRONT,
		BACK,
		LEFT,
		RIGHT,
		FRONT_LEFT,
		FRONT_RIGHT,
		BACK_RIGHT,
		BACK_LEFT,
		INVALID_SIDE
	};

	extern sf::RenderWindow window;
	extern sf::View camera;

	void refreshFrame(std::vector<City*> cities);
	void displayCity(City* city);
	void displayPoint(ConnectionPoint* point, City* city);
	void displayStreet(Street* street);
	ConnectionPoint* getDisplayedRelatedPoint(ConnectionPoint* point);
	Object* isSomethingInTheWay(const sf::Vector2f& cord1, const sf::Vector2f& cord2, City* city, sf::Sprite* relatedPointShape);
	sf::Vector2f getSidePointOfShape(sf::Sprite* shape, DisplaySide side);
	sf::Vector2f getTopLeft(sf::Sprite* shape);
	sf::Vector2f getTopRight(sf::Sprite* shape);
	sf::Vector2f getBotLeft(sf::Sprite* shape);
	sf::Vector2f getBotRight(sf::Sprite* shape);
	sf::Vector2f getMiddlePoint(const sf::Vector2f& point1, const sf::Vector2f& point2);
	sf::Vector2f* convertDirectionToCord(sf::Sprite* shape, const std::vector<Display::DisplaySide>& directions);
	std::vector<Display::DisplaySide> checkRadiusForDirection(sf::Sprite* relatedShape, City* city);
	void checkRadiusSide(const float& xT, const float& yT, sf::Sprite* relatedShape, City* city, std::vector<Display::DisplaySide>& directions);
	DisplaySide getSideByTransformators(const float& xT, const float& yT);
	void getTransformatorBySide(Display::DisplaySide side, float& xT, float& yT);
	float getSmallerFloat(const float number1, const float number2);
	float getGreaterFloat(const float number1, const float number2);
	float getXRadiusBound();
	float getYRadiusBound();
	Object* getObjectByMouse(const sf::Vector2i mousePos, City* city);

	std::string sideToString(DisplaySide side);
}