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

void Display::ConnectionPoint::connectToShape(Street* connectTo, DisplaySide side)
{
	// if connectios.empty == true -> snap point to street
	// else snap street to it

	sf::Vector2f* connectTo_sidePoint = Display::getSidePoints(&connectTo->shape, side);
	sf::Vector2f connectTo_originalOrigin = connectTo->shape.getOrigin();

	DisplaySide connectionPoint_side = this->getRandomFreeSide();
	if (connectionPoint_side == Display::INVALID_SIDE)
	{
		throw CustomExceptions::CannotAddStreetToPoint("Display::ConnectionPoint::connectToShape");
		return;
	}

	sf::Vector2f* connectionPoint_sidePoint = Display::getSidePoints(&this->shape, connectionPoint_side);
	sf::Vector2f connectionPoint_originalOrigin = this->shape.getOrigin();

	// decide to snap to street or snap street to it
	if (this->connections.isEmpty())
	{
		this->connections.addConnection(connectionPoint_side, connectTo);
		this->shape.setOrigin(*connectionPoint_sidePoint);
		this->shape.setPosition(connectTo_sidePoint->x, connectTo_sidePoint->y);
		this->shape.setOrigin(connectionPoint_originalOrigin);
	}
	else
	{
		this->connections.addConnection(connectionPoint_side, connectTo);
		connectTo->isConnectedToPoint_Front = true;
		connectTo->shape.setOrigin(*connectTo_sidePoint);
		connectTo->shape.setPosition(connectionPoint_sidePoint->x, connectionPoint_sidePoint->y);
		connectTo->shape.setOrigin(connectTo_originalOrigin);
	}
}

Display::DisplaySide Display::ConnectionPoint::getRandomFreeSide()
{
	if (this->connections.isFree(Display::LEFT))
	{
		return Display::LEFT;
	}
	else if (this->connections.isFree(Display::RIGHT))
	{
		return Display::RIGHT;
	}
	else if (this->connections.isFree(Display::FRONT))
	{
		return Display::FRONT;
	}
	else if (this->connections.isFree(Display::BACK))
	{
		return Display::BACK;
	}
	else
	{
		return Display::INVALID_SIDE;
	}
}

void Display::displayStreet(Street* street, Street* connectTo)
{
	street->shape.setTexture(*Assets::getObjectTexture("street"));
	sf::FloatRect bounds = street->shape.getGlobalBounds();
	street->shape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	street->isDisplayed = true;

	if (street->root)
	{
		float xPos = xRootPosition;
		float yPos = yRootPosition;
		street->shape.setPosition(xPos, yPos);
	}
	else // get where to snap it
	{
		CityFunctions::VectorSide vectorSide = CityFunctions::getWhereToSnap(street, connectTo);
		Display::DisplaySide side = Display::convertVectorSideToDisplaySide(vectorSide);

	}

	Display::createPointsToStreet(street);

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

sf::Vector2f* Display::getSidePoints(sf::Sprite* shape, DisplaySide side)
{
	switch (side)
	{
	case Display::BACK:
	{
		sf::Vector2f botLeft = Display::getBotLeft(shape);
		sf::Vector2f botRight = Display::getBotRight(shape);
		sf::Vector2f* smaller = new sf::Vector2f(*Display::getSmallerX_Vector2f(&botLeft, &botRight), *Display::getSmallerY_Vector2f(&botLeft, &botRight));

		smaller->x += abs(botLeft.x - botRight.x);
		smaller->y += abs(botLeft.y - botRight.y);

		return smaller;
	}
	case Display::FRONT:
	{
		sf::Vector2f topLeft = Display::getTopLeft(shape);
		sf::Vector2f topRight = Display::getTopRight(shape);
		sf::Vector2f* smaller = new sf::Vector2f(*Display::getSmallerX_Vector2f(&topLeft, &topRight), *Display::getSmallerY_Vector2f(&topLeft, &topRight));

		smaller->x += abs(topLeft.x - topRight.x);
		smaller->y += abs(topLeft.y - topRight.y);

		return smaller;
	}
	case Display::LEFT:
	{
		sf::Vector2f topLeft = Display::getTopLeft(shape);
		sf::Vector2f botLeft = Display::getBotLeft(shape);
		sf::Vector2f* smaller = new sf::Vector2f(*Display::getSmallerX_Vector2f(&topLeft, &botLeft), *Display::getSmallerY_Vector2f(&topLeft, &botLeft));

		smaller->x += abs(topLeft.x - botLeft.x);
		smaller->y += abs(topLeft.y - botLeft.y);

		return smaller;
	}
	case Display::RIGHT:
	{
		sf::Vector2f topRight = Display::getTopRight(shape);
		sf::Vector2f botRight = Display::getBotRight(shape);
		sf::Vector2f* smaller = new sf::Vector2f(*Display::getSmallerX_Vector2f(&topRight, &botRight), *Display::getSmallerY_Vector2f(&topRight, &botRight));

		smaller->x += abs(topRight.x - botRight.x);
		smaller->y += abs(topRight.y - botRight.y);

		return smaller;
	}
	}
}

sf::Vector2f Display::getTopRight(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return *new sf::Vector2f(bounds.left + bounds.width, bounds.top);
}

sf::Vector2f Display::getBotRight(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return *new sf::Vector2f(bounds.left, bounds.top - bounds.height);
}

sf::Vector2f Display::getBotLeft(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return *new sf::Vector2f(bounds.left + bounds.width, bounds.top - bounds.height);
}

sf::Vector2f Display::getTopLeft(sf::Sprite* shape)
{
	sf::FloatRect bounds = shape->getGlobalBounds();
	return *new sf::Vector2f(bounds.left, bounds.top);
}

float* Display::getSmallerX_Vector2f(sf::Vector2f* vec1, sf::Vector2f* vec2)
{
	if (vec1->x < vec2->x)
	{
		return &vec1->x;
	}
	else if (vec1->x > vec2->x)
	{
		return &vec2->x;
	}
	else
	{
		return nullptr;
	}
}

float* Display::getSmallerY_Vector2f(sf::Vector2f* vec1, sf::Vector2f* vec2)
{
	if (vec1->y < vec2->y)
	{
		return &vec1->y;
	}
	else if (vec1->y > vec2->y)
	{
		return &vec2->y;
	}
	else
	{
		return nullptr;
	}
}

Display::DisplaySide Display::getOppositeSide(DisplaySide side)
{
	switch (side)
	{
	case Display::BACK:
		return Display::FRONT;
	case Display::FRONT:
		return Display::BACK;
	case Display::LEFT:
		return Display::RIGHT;
	case Display::RIGHT:
		return Display::LEFT;
	}
}

void Display::createPointsToStreet(Street* street)
{
	if (street->root)
	{
		Display::ConnectionPoint* frontPoint = Display::createConnectionPoint();
		Display::ConnectionPoint* backPoint = Display::createConnectionPoint();
		frontPoint->connectToShape(street, Display::FRONT);
		backPoint->connectToShape(street, Display::BACK);
		street->frontPoint = frontPoint;
		street->backPoint = backPoint;
		street->isConnectedToPoint_Back = true;
		street->isConnectedToPoint_Front = true;
	}
	else if (street->isConnectedToPoint_Front)
	{
		Display::ConnectionPoint* backPoint = Display::createConnectionPoint();
		backPoint->connectToShape(street, Display::BACK);
		street->backPoint = backPoint; 
		street->isConnectedToPoint_Back = true;
	}
	else if (street->isConnectedToPoint_Back)
	{
		Display::ConnectionPoint* frontPoint = Display::createConnectionPoint();
		frontPoint->connectToShape(street, Display::FRONT);
		street->frontPoint = frontPoint;
		street->isConnectedToPoint_Front = true;
	}
}

Display::ConnectionPoint* Display::createConnectionPoint()
{
	ConnectionPoint* res = new ConnectionPoint();
	res->shape.setTexture(*Assets::getObjectTexture("connection_point"));
	res->shape.setOrigin(res->shape.getGlobalBounds().width / 2, res->shape.getGlobalBounds().height / 2);
	return res;
}

Display::DisplaySide Display::convertVectorSideToDisplaySide(CityFunctions::VectorSide side)
{
	if (side == CityFunctions::VECTOR_BACK)
	{
		return Display::BACK;
	}
	else
	{
		return Display::FRONT;
	}
}
