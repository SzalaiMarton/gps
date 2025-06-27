#include "display.h"
#include "assets.h"
#include "../city/new_city.h"
#include <iostream>
#include <cmath>
#include <random>

namespace Display
{
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(800, 800 * (16 / 9)), "gps");
	sf::View camera = sf::View(sf::FloatRect(0, 0, 800, 800 * (16 / 9)));
}

void Display::refreshFrame(std::vector<City*> cities)
{
	Display::window.clear();
	for (auto& city : cities)
	{
		for (auto& str : city->streets)
		{
			Display::window.draw(*str->shape);
		}
		for (auto& point : city->points)
		{
			Display::window.draw(*point->shape);
		}
	}
	Display::window.display();
}

void Display::displayCity(City* city)
{
	ConnectionPoint* firstPoint = *city->points.begin();
	firstPoint->shape->setPosition(0.f,0.f);
	firstPoint->isDisplayed = true;

	for (auto& point : city->points)
	{
		Display::displayPoint(point, city);
	}
	std::cout << "----------------------------------------" << std::endl;

	bool found = false;
	do
	{
		found = false;
		std::cout << city->toBeMovedPoints.size() << std::endl;
		for (auto& point : city->toBeMovedPoints)
		{
			city->connectTwoPoints(point);
			Display::displayPoint(point, city);

			if (point->shape->getPosition().x != 0.f && point->shape->getPosition().y != 0.f)
			{
				for (int i = 0; i < city->toBeMovedPoints.size(); i++)
				{
					if (city->toBeMovedPoints[i] == point)
					{
						city->toBeMovedPoints.erase(city->toBeMovedPoints.begin() + i);
						found = true;
						break;
					}
				}
			}

			if (found)
				break;
		}
	} while (!city->toBeMovedPoints.empty());


	/*for (auto& str : city->streets)
	{
		Display::displayStreet(str);
	}*/

	for (auto it = city->streets.begin(); it != city->streets.end(); ) {
		Street* street = *it;
		if (street->backPoint == nullptr && street->frontPoint == nullptr) 
		{
			delete street;
			it = city->streets.erase(it);
		}
		else 
		{
			++it;
		}
	}
}

void Display::displayPoint(ConnectionPoint* point, City* city)
{
	ConnectionPoint* relatedPoint = Display::getDisplayedRelatedPoint(point);

	if (relatedPoint == nullptr)
	{
		std::cout << point->name << "'s related was null" << std::endl;
		return;
	}

	sf::Vector2f* pos = Display::convertDirectionToCord(relatedPoint->shape, Display::checkRadiusForDirection(relatedPoint->shape, city));
	if (pos == nullptr)
	{
		std::cout << point->name << "'s pos null" << std::endl;
		relatedPoint->removeRelatedPoint(point, city);
		city->toBeMovedPoints.emplace_back(point);
		return;
	}

	point->shape->setTexture(*Assets::getObjectTexture(Assets::pointTextureName));
	point->shape->setOrigin(point->shape->getGlobalBounds().width / 2.f, point->shape->getGlobalBounds().height / 2.f);
	point->shape->setPosition(*pos);
	delete pos;
	point->isDisplayed = true;
	Display::window.draw(*point->shape);
	Display::window.display();

	return;
}

void Display::displayStreet(Street* street)
{
    street->shape->setTexture(*Assets::getObjectTexture(Assets::streetTextureName));

    if (street->backPoint == nullptr || street->frontPoint == nullptr)
        return;

    sf::Vector2f cordBack = street->backPoint->shape->getPosition();
    sf::Vector2f cordFront = street->frontPoint->shape->getPosition();

    street->shape->setOrigin(Display::getSidePointOfShape(street->shape, BACK));
    street->shape->setPosition(cordBack);
    
    float xLength = cordBack.x - cordFront.x;
    float yLength = cordBack.y - cordFront.y;

    float angleRad = std::atan2(yLength, xLength);
	float angle = (angleRad * 180.0f / 3.14f) /*+ 90.f*/;
    street->shape->setRotation(angle);

    float stretch = std::sqrt(xLength * xLength + yLength * yLength) / Assets::streetSizeY;
    street->shape->setScale(1.f, stretch);
}

ConnectionPoint* Display::getDisplayedRelatedPoint(ConnectionPoint* point)
{
	for (auto& pt : point->connectedPoints)
	{
		if (pt->isDisplayed)
			return pt;
	}
	return nullptr;
}

Object* Display::isSomethingInTheWay(const sf::Vector2f& cord1, const sf::Vector2f& cord2, City* city, sf::Sprite* relatedPointShape)
{
	float smallerX = Display::getSmallerFloat(cord1.x, cord2.x);
	float smallerY = Display::getSmallerFloat(cord1.y, cord2.y);
	
	float greaterX = Display::getGreaterFloat(cord1.x, cord2.x);
	float greaterY = Display::getGreaterFloat(cord1.y, cord2.y);

	for (auto& point : city->points)
	{
		if (point->shape == relatedPointShape)
			continue;

		const sf::Vector2f& currentPointPos = point->shape->getPosition();
		if ((currentPointPos.x <= greaterX && currentPointPos.y <= greaterY) && (currentPointPos.x >= smallerX && currentPointPos.y >= smallerY) && point->isDisplayed)
		{
			return point;
		}
	}
	for (auto& str : city->streets)
	{
		const sf::Vector2f& currentStrPos = str->shape->getPosition();
		if ((currentStrPos.x <= greaterX && currentStrPos.y <= greaterY) && (currentStrPos.x >= smallerX && currentStrPos.y >= smallerY) && str->isDisplayed)
		{
			return str;
		}
	}
	return nullptr;
}

sf::Vector2f Display::getSidePointOfShape(sf::Sprite* shape, DisplaySide side)
{
	if (side == FRONT)
	{
		return sf::Vector2f(Display::getMiddlePoint(Display::getTopLeft(shape), Display::getTopRight(shape)));
	}
	if (side == BACK)
	{
		return sf::Vector2f(Display::getMiddlePoint(Display::getBotLeft(shape), Display::getBotRight(shape)));
	}
	if (side == LEFT)
	{
		return sf::Vector2f(Display::getMiddlePoint(Display::getTopLeft(shape), Display::getBotLeft(shape)));
	}
	if (side == RIGHT)
	{
		return sf::Vector2f(Display::getMiddlePoint(Display::getTopRight(shape), Display::getBotRight(shape)));
	}
	return sf::Vector2f(0.f,0.f);
}

sf::Vector2f Display::getTopLeft(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return sf::Vector2f(bounds.left, bounds.top);
}

sf::Vector2f Display::getTopRight(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return sf::Vector2f(bounds.left + bounds.width, bounds.top);
}

sf::Vector2f Display::getBotLeft(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return sf::Vector2f(bounds.left, bounds.top + bounds.height);
}

sf::Vector2f Display::getBotRight(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return sf::Vector2f(bounds.left + bounds.width, bounds.top + bounds.height);
}

sf::Vector2f Display::getMiddlePoint(const sf::Vector2f& point1, const sf::Vector2f& point2)
{
	sf::Vector2f smaller(Display::getSmallerFloat(point1.x, point2.x), Display::getSmallerFloat(point1.y, point2.y));

	smaller.x += abs(point1.x - point2.x) / 2.f;
	smaller.y += abs(point1.y - point2.y) / 2.f;

	if (point1.x == point2.x)
		smaller.x = point1.x;
	if (point1.y == point2.y)
		smaller.y = point1.y;

	return smaller;
}

sf::Vector2f* Display::convertDirectionToCord(sf::Sprite* shape, const std::vector<Display::DisplaySide>& directions)
{
	if (directions.empty())
		return nullptr;

	sf::Vector2f pos = shape->getPosition();
	float xT = 0.f, yT = 0.f;
	Display::getTransformatorBySide(directions[0], xT, yT);

	return new sf::Vector2f(pos.x + (Display::getXRadiusBound() * xT), pos.y + (Display::getYRadiusBound() * yT));
}

std::vector<Display::DisplaySide> Display::checkRadiusForDirection(sf::Sprite* relatedShape, City* city)
{
	std::vector<Display::DisplaySide> directions{};

	// RIGHT
	checkRadiusSide(1.f, 0.f, relatedShape, city, directions);
	// LEFT
	checkRadiusSide(-1.f, 0.f, relatedShape, city, directions);
	// BACK
	checkRadiusSide(0.f, 1.f, relatedShape, city, directions);
	// FRONT
	checkRadiusSide(0.f, -1.f, relatedShape, city, directions);

	//// FRONT RIGHT
	//checkRadiusSide(2.f, -2.f, relatedShape, city, directions);
	//// FRONT LEFT
	//checkRadiusSide(-2.f, -2.f, relatedShape, city, directions);
	//// BACK RIGHT
	//checkRadiusSide(2.f, 2.f, relatedShape, city, directions);
	//// BACK LEFT
	//checkRadiusSide(-2.f, 2.f, relatedShape, city, directions);

	return directions;
}

void Display::checkRadiusSide(const float& xT, const float& yT, sf::Sprite* relatedShape, City* city, std::vector<Display::DisplaySide>& directions)
{
	sf::Vector2f shapePos = relatedShape->getPosition();
	sf::Vector2f radiusBound(0.f, 0.f);

	radiusBound.x = shapePos.x + (Display::getXRadiusBound() * xT);
	radiusBound.y = shapePos.y + (Display::getYRadiusBound() * yT);

	Object* obj = Display::isSomethingInTheWay(shapePos, radiusBound, city, relatedShape);
	if (obj == nullptr)
		directions.emplace_back(Display::getSideByTransformators(xT, yT));
}

Display::DisplaySide Display::getSideByTransformators(const float& xT, const float& yT)
{
	if (xT == 1.f && yT == 0.f)
		return RIGHT;
	else if (xT == -1.f && yT == 0.f)
		return LEFT;
	else if (xT == 0.f && yT == 1.f)
		return BACK;
	else if (xT == 0.f && yT == -1.f)
		return FRONT;

	else if (xT == 2.f && yT == 2.f)
		return BACK_RIGHT;
	else if (xT == -2.f && yT == 2.f)
		return BACK_LEFT;
	else if (xT == 2.f && yT == -2.f)
		return FRONT_RIGHT;
	else if (xT == -2.f && yT == -2.f)
		return FRONT_LEFT;
	else
		return INVALID_SIDE;
}

void Display::getTransformatorBySide(Display::DisplaySide side, float& xT, float& yT)
{
	if (side == RIGHT)
	{
		xT = 1.f;
		yT = 0.f;
	}
	else if (side == LEFT)
	{
		xT = -1.f;
		yT = 0.f;
	}
	else if (side == BACK)
	{
		xT = 0.f;
		yT = 1.f;
	}
	else if (side == FRONT)
	{
		xT = 0.f;
		yT = -1.f;
	}

	else if (side == BACK_RIGHT)
	{
		xT = 2.f;
		yT = 2.f;
	}
	else if (side == BACK_LEFT)
	{
		xT = -2.f;
		yT = 2.f;
	}
	else if (side == FRONT_RIGHT)
	{
		xT = 2.f;
		yT = -2.f;
	}
	else if (side == FRONT_LEFT)
	{
		xT = -2.f;
		yT = -2.f;
	}
}

float Display::getSmallerFloat(const float number1, const float number2)
{
	if (number1 < number2)
		return number1;
	else if (number1 > number2)
		return number2;
	else
		return number1;
}

float Display::getGreaterFloat(const float number1, const float number2)
{
	if (number1 > number2)
		return number1;
	else if (number1 < number2)
		return number2;
	else
		return number1;
}

float Display::getXRadiusBound()
{
	return Assets::streetSizeY * 2.f;
}

float Display::getYRadiusBound()
{
	return Assets::streetSizeY * 2.f;
}

Object* Display::getObjectByMouse(const sf::Vector2i mousePos, City* city)
{
	for (auto& str : city->streets)
	{
		if (str->shape->getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
			return str;
	}
	for (auto& point : city->points)
	{
		if (point->shape->getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
			return point;
	}
	return nullptr;
}

std::string Display::sideToString(DisplaySide side)
{
	switch (side)
	{
	case RIGHT:
		return "right";
	case LEFT:
		return "left";
	case FRONT:
		return "front";
	case BACK:
		return "back";
	case FRONT_RIGHT:
		return "front_right";
	case FRONT_LEFT:
		return "front_left";
	case BACK_LEFT:
		return "back_left";
	case BACK_RIGHT:
		return "back_right";
	default:
		return "invalid_side";
	}
}
