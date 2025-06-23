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
	void displayPoint(ConnectionPoint* point, City* city, bool tryAgain, int& tryCount);
	void displayStreet(Street* street);
	void getDisplayedRelatedPoint(ConnectionPoint* point, ConnectionPoint** relatedPoint);
	Object* isSomethingInTheWay(sf::Vector2f* pos1, sf::Vector2f* pos2, City* city);
	sf::Vector2f* getSmallerVector2f(sf::Vector2f* pos1, sf::Vector2f* pos2);
	sf::Vector2f* getGreaterVector2f(sf::Vector2f* pos1, sf::Vector2f* pos2);
	sf::Vector2f getSidePointOfShape(sf::Sprite* shape, DisplaySide side);
	sf::Vector2f getTopLeft(sf::Sprite* shape);
	sf::Vector2f getTopRight(sf::Sprite* shape);
	sf::Vector2f getBotLeft(sf::Sprite* shape);
	sf::Vector2f getBotRight(sf::Sprite* shape);
	sf::Vector2f getMiddlePoint(const sf::Vector2f& point1, const sf::Vector2f& point2);
	sf::Vector2f* convertDirectionToCord(sf::Sprite* shape, const std::vector<Display::DisplaySide>& directions);
	std::vector<Display::DisplaySide> checkRadiusForDirection(sf::Sprite* shape, City* city);
	void checkRadiusSides(const float& xT, const float& yT, sf::Sprite* shape, City* city, std::vector<Display::DisplaySide>& directions);
	DisplaySide getSideByTransformators(const float& xT, const float& yT);
	void getTransformatorBySide(Display::DisplaySide side, float& xT, float& yT);
	float getSmallerFloat(const float number1, const float number2);
	float getXRadiusBound();
	float getYRadiusBound();
	Object* getObjectByMouse(const sf::Vector2i mousePos, City* city);
	void stretchObject(sf::Sprite* shape, float xScale, float yScale);
	void movePointToAnotherPoint(ConnectionPoint* toBeRemoved, ConnectionPoint* relatedPoint, City* city, int& tryCount);

	std::string sideToString(DisplaySide side);
}