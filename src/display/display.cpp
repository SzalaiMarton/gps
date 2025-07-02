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
	Display::displayFirstPoint(city);
	Display::displayNewPoints(city);

	/*for (auto& str : city->streets)
	{
		Display::displayStreet(str);
	}*/

	city->removeUnusedStreets();
	Display::window.display();
}

bool Display::displayPoint(ConnectionPoint* point, City* city)
{
	ConnectionPoint* relatedPoint = Display::getDisplayedRelatedPoint(point);

	if (relatedPoint == nullptr)
	{
		std::cout << "[ERROR] " << point->name << "'s related point was null." << std::endl;
		return false;
	}

	sf::Vector2f pos = Display::convertDirectionToCord(relatedPoint->shape, Display::lookForNearbyEmptySpace(relatedPoint->shape, city));
	if (pos.x == 0.f && pos.y == 0.f)
	{
		std::cout << point->name << " pos was null" << std::endl;
		relatedPoint->noSpaceAround = true;
		CityFunctions::disconnectTwoPoints(point, relatedPoint, city);
		Display::movePoint(point, city);
		return false;
	}
	else
	{
		point->shape->setTexture(*Assets::getObjectTexture(Assets::pointTextureName));
		point->shape->setOrigin(point->shape->getGlobalBounds().width / 2.f, point->shape->getGlobalBounds().height / 2.f);
		point->shape->setPosition(pos);
		Display::window.draw(*point->shape);
		return true;
	}
}

void Display::displayFirstPoint(City* city)
{
	ConnectionPoint* firstPoint = *city->points.begin();
	firstPoint->shape->setTexture(*Assets::getObjectTexture(Assets::pointTextureName));
	firstPoint->shape->setOrigin(firstPoint->shape->getGlobalBounds().width / 2.f, firstPoint->shape->getGlobalBounds().height / 2.f);
	firstPoint->shape->setPosition(0.f, 0.f);
	city->toBeDisplayedPoints.erase(std::find(city->toBeDisplayedPoints.begin(), city->toBeDisplayedPoints.end(), firstPoint));
	city->firstDisplayPoint = firstPoint;
	Display::window.draw(*firstPoint->shape);
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
	float angle = (angleRad * 180.0f / 3.14f);
    street->shape->setRotation(angle);

    float stretch = std::sqrt(xLength * xLength + yLength * yLength) / Assets::streetSizeY;
    street->shape->setScale(1.f, stretch);
}

void Display::displayNewPoints(City* city)
{
	std::vector<ConnectionPoint*> parentPoints = { city->firstDisplayPoint }; // already displayed points
	std::vector<ConnectionPoint*> childPoints{}; // yet to be displayed points
	do
	{
		childPoints.clear();

		for (auto& parentP : parentPoints)
		{
			for (auto& parentConnections : parentP->connectedPoints)
			{
				if (!parentConnections->isDisplayed() && parentConnections != parentP)
					childPoints.emplace_back(parentConnections);
			}
		}

		parentPoints.clear();

		for (auto& childP : childPoints)
		{
			if (Display::displayPoint(childP, city))
			{
				parentPoints.emplace_back(childP);
			}
		}
	} while (!childPoints.empty());
}

ConnectionPoint* Display::getDisplayedRelatedPoint(ConnectionPoint* point)
{
	for (auto& pt : point->connectedPoints)
	{
		if (pt->isDisplayed())
		{
			return pt;
		}
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
		if ((currentPointPos.x <= greaterX && currentPointPos.y <= greaterY) && (currentPointPos.x >= smallerX && currentPointPos.y >= smallerY) && point->isDisplayed())
		{
			return point;
		}
	}
	for (auto& str : city->streets)
	{
		const sf::Vector2f& currentStrPos = str->shape->getPosition();
		if ((currentStrPos.x <= greaterX && currentStrPos.y <= greaterY) && (currentStrPos.x >= smallerX && currentStrPos.y >= smallerY) && str->isDisplayed())
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

sf::Vector2f Display::convertDirectionToCord(sf::Sprite* shape, const std::vector<Display::DisplaySide>& directions)
{
	if (directions.empty())
		return sf::Vector2f();

	sf::Vector2f pos = shape->getPosition();
	float xT = 0.f, yT = 0.f;
	Display::getTransformatorBySide(directions[0], xT, yT);

	return sf::Vector2f(pos.x + (Display::getXRadiusBound() * xT), pos.y + (Display::getYRadiusBound() * yT));
}

bool Display::movePoint(ConnectionPoint* toBeMovedPoint, City* city)
{
	std::vector<DisplaySide> directions;

	/*ConnectionPoint* newRelatedPoint = nullptr;
	while (true)
	{
		newRelatedPoint = city->getRandomPoint(0);
		if (newRelatedPoint->noSpaceAround || newRelatedPoint->isFull || !newRelatedPoint->isDisplayed)
			continue;

		directions = Display::lookForNearbyEmptySpace(newRelatedPoint->shape, city);
		if (!directions.empty())
		{
			CityFunctions::connectTwoPoints(toBeMovedPoint, newRelatedPoint, city);
			break;
		}

		newRelatedPoint->noSpaceAround = true;
	}*/

	for (auto& newRelatedPoint : city->points)
	{
		if (newRelatedPoint->noSpaceAround || newRelatedPoint->isFull() || !newRelatedPoint->isDisplayed())
			continue;

		directions = Display::lookForNearbyEmptySpace(newRelatedPoint->shape, city);
		if (!directions.empty())
		{
			CityFunctions::connectTwoPoints(toBeMovedPoint, newRelatedPoint, city);
			return true;
		}

		newRelatedPoint->noSpaceAround = true;
	}
	std::cout << "[ERROR] city is out of unfinished points." << std::endl;
	return false;
}

std::vector<Display::DisplaySide> Display::lookForNearbyEmptySpace(sf::Sprite* relatedShape, City* city)
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
	default:
		return "invalid_side";
	}
}
