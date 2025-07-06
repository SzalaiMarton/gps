#include "display.h"
#include "assets.h"
#include "../city/new_city.h"
#include <iostream>
#include <cmath>
#include <random>
#include <unordered_map>
#include <utility>

namespace Display
{
	uint16_t winSize = 800;
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode(winSize, winSize * (16 / 9)), "gps");
	sf::View camera = sf::View(sf::FloatRect(0, 0, winSize, winSize * (16 / 9)));
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

	for (auto& str : city->streets)
	{
		Display::displayStreet(str);
	}

	DEBUG_LOG("Removal of unused streets began.");
	city->removeUnusedStreets();
	Display::window.display();
}

bool Display::displayPoint(ConnectionPoint* point, City* city)
{
	std::vector<ConnectionPoint*> relatedPointsVector = Display::getDisplayedRelatedPoint(point);
	if (relatedPointsVector.empty())
	{
		DEBUG_LOG("[ERROR] " << point->name << "'s related point was null.");
		return false;
	}
	std::vector<ConnectionPoint*> removeRelated{};
	ConnectionPoint* optimalRelated = Display::getOptimalRelatedPoint(relatedPointsVector, removeRelated);
	for (auto& removeP : removeRelated)
	{
		CityFunctions::disconnectTwoPoints(point, removeP, city);
	}

	std::vector<Display::DisplaySide> emptySpaces = Display::lookForNearbyEmptySpace(optimalRelated->shape, city);
	if (emptySpaces.empty())
	{
		DEBUG_LOG("there were no empty space for " << point->name);
		optimalRelated->noSpaceAround = true;
		CityFunctions::disconnectTwoPoints(point, optimalRelated, city);
		LOG(optimalRelated->name << " has no space around.");
		Display::movePoint(point, city);
		return false;
	}
	else
	{
		sf::Vector2f pos = Display::convertDirectionToCord(optimalRelated->shape, emptySpaces[0]);
		point->shape->setTexture(*Assets::getObjectTexture(Assets::pointTextureName));
		point->shape->setOrigin(point->shape->getGlobalBounds().width / 2.f, point->shape->getGlobalBounds().height / 2.f);
		point->shape->setPosition(pos);
		Display::window.draw(*point->shape);
	}

	if (point->connectedPoints.size() < point->maxConnection)
	{
		LOG("Atempting to connect " << point->name << " with close points");
		Display::connectPointWithClosePoints(point, city, false);
	}
	return true;
}

void Display::displayFirstPoint(City* city)
{
	if (city->points.empty())
	{
		DEBUG_LOG("[EMPTY CITY] City's point vector was empty");
		return;
	}

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
	{
		DEBUG_LOG(street->name << " isn't a full street, either back- or frontPoint is missing.");
		return;
	}

    sf::Vector2f cordBack = street->backPoint->shape->getPosition();
    sf::Vector2f cordFront = street->frontPoint->shape->getPosition();

    street->shape->setOrigin(Display::getSidePointOfShape(street->shape, BACK));
    street->shape->setPosition(cordBack);

    float xLength = cordBack.x - cordFront.x;
    float yLength = cordBack.y - cordFront.y;

    float angleRad = std::atan2(yLength, xLength);
	float angle = (angleRad * 180.0f / 3.14f) - 90.f;
    street->shape->setRotation(angle);

    float stretch = std::sqrt(xLength * xLength + yLength * yLength) / Assets::streetSizeY;
    street->shape->setScale(1.f, stretch);
}

void Display::displayNewPoints(City* city)
{
	std::vector<ConnectionPoint*> parentPoints = { city->firstDisplayPoint }; // already displayed points
	std::vector<ConnectionPoint*> childPoints{}; // yet to be displayed points
	DEBUG_LOG("Displaying new points started...");
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

	for (auto& p : city->points)
	{
		Display::removeDistantPoint(p, city);
		if (p->connectedPoints.size() < p->maxConnection)
		{
			Display::connectPointWithClosePoints(p, city, true);
		}
	}

	DEBUG_LOG("Displaying new points finished.");
}

ConnectionPoint* Display::getOptimalRelatedPoint(std::vector<ConnectionPoint*>& points, std::vector<ConnectionPoint*>& toBeRemoved)
{
	if (points.size() == 1)
		return points[0];
	
	std::vector<std::pair<ConnectionPoint*, ConnectionPoint*>> validPairs{};
	std::unordered_map<ConnectionPoint*, int> appearanceCount{};

	for (auto rootIT = points.begin(); rootIT != points.end(); rootIT++)
	{
		appearanceCount[*rootIT] = 0;
		for (auto innerIT = rootIT + 1; innerIT != points.end(); innerIT++)
		{
			if (*rootIT == *innerIT)
				continue;

			sf::Vector2f rootPpos = (*rootIT)->shape->getPosition();
			sf::Vector2f innerPpos = (*innerIT)->shape->getPosition();
			float distance = sqrt(pow(abs(rootPpos.y - innerPpos.y), 2) + pow(abs(rootPpos.x - innerPpos.x), 2));

			if (distance <= Display::getRadiusBound())
			{
				validPairs.emplace_back(*rootIT, *innerIT);
			}
		}
	}

	for (auto& innerVector : validPairs)
	{
		appearanceCount[innerVector.first] += 1;
		appearanceCount[innerVector.second] += 1;
	}

	ConnectionPoint* highestAppearance = points[0];

	for (auto& p : appearanceCount)
	{
		if (p.second == 0)
			toBeRemoved.emplace_back(p.first);
		if (appearanceCount[highestAppearance] < appearanceCount[p.first])
			highestAppearance = p.first;
	}
	return highestAppearance;
}

std::vector<ConnectionPoint*> Display::getDisplayedRelatedPoint(ConnectionPoint* point)
{
	std::vector<ConnectionPoint*> res{};
	for (auto& pt : point->connectedPoints)
	{
		if (pt->isDisplayed())
		{
			res.emplace_back(pt);
		}
	}
	return res;
}

Object* Display::isSomethingInTheWay(const sf::Vector2f& cord1, const sf::Vector2f& cord2, City* city, sf::Sprite* ignoreShape)
{
	float smallerX = Display::getSmallerFloat(cord1.x, cord2.x);
	float smallerY = Display::getSmallerFloat(cord1.y, cord2.y);
	
	float greaterX = Display::getGreaterFloat(cord1.x, cord2.x);
	float greaterY = Display::getGreaterFloat(cord1.y, cord2.y);

	for (auto& point : city->points)
	{
		if (point->shape == ignoreShape)
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

sf::Vector2f Display::convertDirectionToCord(sf::Sprite* rootShape, const Display::DisplaySide& directions)
{
	sf::Vector2f pos = rootShape->getPosition();
	float xT = 0.f, yT = 0.f;
	Display::getTransformatorBySide(directions, xT, yT);

	return sf::Vector2f(pos.x + (Display::getRadiusBound() * xT), pos.y + (Display::getRadiusBound() * yT));
}

bool Display::movePoint(ConnectionPoint* toBeMovedPoint, City* city)
{
	std::vector<DisplaySide> directions;
	bool outOfCandidates = false;
	/*for (auto& newRelatedPoint : city->points)
	{
		if (newRelatedPoint->noSpaceAround || newRelatedPoint->isFull() || !newRelatedPoint->isDisplayed())
			continue;

		directions = Display::lookForNearbyEmptySpace(newRelatedPoint->shape, city);
		if (!directions.empty())
		{
		
		}

		newRelatedPoint->noSpaceAround = true;
	}

	DEBUG_LOG("[FAILED TO MOVE POINT] out of candidates while moving " << toBeMovedPoint->name);
	return false;
	*/

	LOG("Moving " << toBeMovedPoint->name);
	CityFunctions::connectTwoPoints(toBeMovedPoint, nullptr, city, outOfCandidates);
	if (outOfCandidates)
	{
		DEBUG_LOG("[FAILED TO MOVE POINT] out of candidates while moving " << toBeMovedPoint->name);
		return false;
	}
	LOG("Finished moving " << toBeMovedPoint->name);
	return true;
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

void Display::connectPointWithClosePoints(ConnectionPoint* point, City* city, bool forceConnection)
{
	std::vector<Display::DisplaySide> initDirections = {Display::FRONT,Display::RIGHT, Display::LEFT, Display::BACK};
	std::vector<Display::DisplaySide> emptySpace = Display::lookForNearbyEmptySpace(point->shape, city);
	bool outOfPoints = false;
	for (auto& dir : initDirections)
	{
		if (std::find(emptySpace.begin(), emptySpace.end(), dir) != emptySpace.end())
		{
			if (std::find(initDirections.begin(), initDirections.end(), dir) != initDirections.end())
			{
				initDirections.erase(std::find(initDirections.begin(), initDirections.end(), dir));
			}
		}
		else
		{
			ConnectionPoint* nearbyP = dynamic_cast<ConnectionPoint*>(Display::isSomethingInTheWay(Display::convertDirectionToCord(point->shape, dir), point->shape->getPosition(), city, point->shape));
			if (nearbyP != nullptr && !point->isPointAlreadyConnected(nearbyP))
			{
				if (forceConnection)
					nearbyP->maxConnection++;
				CityFunctions::connectTwoPoints(point, nearbyP, city, outOfPoints);
			}
		}
	}
}

void Display::removeDistantPoint(ConnectionPoint* rootPoint, City* city)
{
	for (auto& connectedP : rootPoint->connectedPoints)
	{
		sf::Vector2f pPos = rootPoint->shape->getPosition();
		sf::Vector2f connectedPos = connectedP->shape->getPosition();
		float distance = sqrt(pow(abs(pPos.x - connectedPos.x), 2) + pow(abs(pPos.y - connectedPos.y), 2));
		if (distance > Display::getRadiusBound())
		{
			CityFunctions::disconnectTwoPoints(rootPoint, connectedP, city);
		}
	}
}

void Display::checkRadiusSide(const float& xT, const float& yT, sf::Sprite* relatedShape, City* city, std::vector<Display::DisplaySide>& directions)
{
	sf::Vector2f shapePos = relatedShape->getPosition();
	sf::Vector2f radiusBound(0.f, 0.f);

	radiusBound.x = shapePos.x + (Display::getRadiusBound() * xT);
	radiusBound.y = shapePos.y + (Display::getRadiusBound() * yT);

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

float Display::getRadiusBound()
{
	return Assets::streetSizeY * 2.f;
}

Object* Display::getObjectByMouse(const sf::Vector2i mousePos, City* city)
{
	//for (auto& str : city->streets)
	//{
	//	if (str->shape->getGlobalBounds().contains((float)mousePos.x, (float)mousePos.y))
	//		return str;
	//}
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
