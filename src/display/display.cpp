#include "display.h"
#include "assets.h"
#include "../city/new_city.h"
#include <iostream>

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
	int tryCount = 0;
	for (auto& point : city->points)
	{
		tryCount = 0;
		Display::displayPoint(point, city, true, tryCount);
	}
	/*for (auto& str : city->streets)
	{
		std::cout << "displaying " << str->name << std::endl;
		Display::displayStreet(str);
	}*/
}

void Display::displayPoint(ConnectionPoint* point, City* city, bool tryAgain, int& tryCount)
{
	point->shape->setTexture(*Assets::getObjectTexture(Assets::pointTextureName));

	ConnectionPoint* relatedPoint = nullptr;
	Display::getDisplayedRelatedPoint(point, &relatedPoint);
	if (relatedPoint == nullptr) // if relatedPoint is null that means this is the first point to be displayed
	{
		point->shape->setPosition(0.f, 0.f);
		point->isDisplayed = true;
	}
	else
	{
		// check if fits in the radius, if not place it further
		sf::Vector2f* pos = Display::convertDirectionToCord(relatedPoint->shape, Display::checkRadiusForDirection(relatedPoint->shape, city));
		if (pos == nullptr)
		{
			if (tryAgain)
				Display::movePointToAnotherPoint(point, relatedPoint, city, tryCount);
			else
			{
				std::cout << "pos was null after the " << tryCount << " try" << std::endl;
			}
			return;
		}
		point->shape->setPosition(*pos);
		delete pos;
		point->isDisplayed = true;
	}
	Display::window.draw(*point->shape);
}

void Display::displayStreet(Street* street)
{
	street->shape->setTexture(*Assets::getObjectTexture(Assets::streetTextureName));

	// get the two points of the front and backPoint
	sf::Vector2f cordBack = street->backPoint->shape->getPosition();
	sf::Vector2f cordFront = street->frontPoint->shape->getPosition();

	// place it to the smaller cord
	street->shape->setOrigin(Display::getSidePointOfShape(street->shape, BACK));
	street->shape->setPosition(*Display::getSmallerVector2f(&cordBack, &cordFront));
	
	// get stretch length
	float xLength = abs(cordBack.x - cordFront.x);
	float yLength = abs(cordBack.y - cordFront.y);

	// calculate rotation
	float angle = yLength / xLength;
	std::cout << "angle: " << angle << std::endl;
	street->shape->setRotation(angle);

	
	std::cout << "xLength: " << xLength << std::endl;
	std::cout << "yLength: " << yLength << std::endl;
	Display::stretchObject(street->shape, xLength, yLength);

	
}

void Display::getDisplayedRelatedPoint(ConnectionPoint* point, ConnectionPoint** relatedPoint)
{
	for (auto& str : point->connections)
	{
		if (str->backPoint != nullptr && str->backPoint != point && str->backPoint->isDisplayed)
			*relatedPoint = str->backPoint;
		if (str->frontPoint != nullptr && str->frontPoint != point && str->frontPoint->isDisplayed)
			*relatedPoint = str->frontPoint;
	}
}

Object* Display::isSomethingInTheWay(sf::Vector2f* smaller, sf::Vector2f* greater, City* city)
{
	for (auto& point : city->points)
	{
		const sf::Vector2f& currentPointPos = point->shape->getPosition();
		if ((currentPointPos.x <= greater->x && currentPointPos.y <= greater->y) && (currentPointPos.x >= smaller->x && currentPointPos.y >= smaller->y))
		{
			return point;
		}
	}
	for (auto& str : city->streets)
	{
		const sf::Vector2f& currentStrPos = str->shape->getPosition();
		if ((currentStrPos.x <= greater->x && currentStrPos.y <= greater->y) && (currentStrPos.x >= smaller->x && currentStrPos.y >= smaller->y))
		{
			return str;
		}
	}
	return nullptr;
}

sf::Vector2f* Display::getSmallerVector2f(sf::Vector2f* pos1, sf::Vector2f* pos2)
{
	if (pos1->x < pos2->x && pos1->y < pos2->y)
		return pos1;
	else
		return pos2;
}

sf::Vector2f* Display::getGreaterVector2f(sf::Vector2f* pos1, sf::Vector2f* pos2)
{
	if (pos1->x > pos2->x && pos1->y > pos2->y)
		return pos1;
	else
		return pos2;
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
	// from the vector grab the first element and convert it into a cord relative to the relatedPoint's pos
	if (directions.empty())
		return nullptr;

	sf::Vector2f pos = shape->getPosition();
	float xT = 0.f, yT = 0.f;
	Display::getTransformatorBySide(directions[0], xT, yT);

	return new sf::Vector2f(pos.x + (Display::getXRadiusBound() * xT), pos.y + (Display::getYRadiusBound() * yT));
}

std::vector<Display::DisplaySide> Display::checkRadiusForDirection(sf::Sprite* shape, City* city)
{
	// check all four side and pop related point sides
	std::vector<Display::DisplaySide> directions{};

	// RIGHT
	checkRadiusSides(1.f, 0.f, shape, city, directions);
	// LEFT
	checkRadiusSides(-1.f, 0.f, shape, city, directions);
	// BACK
	checkRadiusSides(0.f, 1.f, shape, city, directions);
	// FRONT
	checkRadiusSides(0.f, -1.f, shape, city, directions);
	
	// FRONT RIGHT
	checkRadiusSides(2.f, -2.f, shape, city, directions);
	// FRONT LEFT
	checkRadiusSides(-2.f, -2.f, shape, city, directions);
	// BACK RIGHT
	checkRadiusSides(2.f, 2.f, shape, city, directions);
	// BACK LEFT
	checkRadiusSides(-2.f, 2.f, shape, city, directions);

	return directions;
}

void Display::checkRadiusSides(const float& xT, const float& yT, sf::Sprite* shape, City* city, std::vector<Display::DisplaySide>& directions)
{
	sf::Vector2f shapePos = shape->getPosition();
	sf::Vector2f radiusBound(0.f, 0.f);

	radiusBound.x = shapePos.x + (Display::getXRadiusBound() * xT);
	radiusBound.y = shapePos.y + (Display::getYRadiusBound() * yT);

	Object* obj = Display::isSomethingInTheWay(Display::getSmallerVector2f(&shapePos, &radiusBound), Display::getGreaterVector2f(&shapePos, &radiusBound), city);
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
		return 0;
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

void Display::stretchObject(sf::Sprite* shape, float xScale, float yScale)
{
	if (xScale == 0.f)
		xScale = 1.f;
	if (yScale == 0.f)
		yScale = 1.f;

	shape->setScale(xScale / shape->getGlobalBounds().width, yScale / shape->getGlobalBounds().height);
}

void  Display::movePointToAnotherPoint(ConnectionPoint* toBeRemoved, ConnectionPoint* relatedPoint, City* city, int& tryCount)
{
	relatedPoint->removeRelatedPoint(toBeRemoved, city);
	//city->getLastCreatedPoint()->connectPoint(toBeRemoved, city);
	city->getRandomPoint()->connectPoint(toBeRemoved, city);
	city->movePointToTheBack(toBeRemoved);
	
	if (tryCount < 5)
	{
		tryCount++;
		Display::displayPoint(toBeRemoved, city, true, tryCount);
	}
	else
		Display::displayPoint(toBeRemoved, city, false, tryCount);
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
