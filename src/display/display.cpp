#include "display.h"
#include "assets.h"

namespace Display
{
	int width = 1000;
	int height = width * (16 / 9);
	sf::RenderWindow window(sf::VideoMode(width, height), "gps");

	float xRootPosition = width / 2.f, yRootPosition = height / 2.f - 200.f;
}

Display::ConnectionPoint::ConnectionPoint()
{
	this->connections = Display::ConnectionsToShape<Street*>();
	this->shape = sf::Sprite();
}

void Display::ConnectionPoint::connectToShape(Street* connectTo, DisplaySide whereTo)
{
	if (this->connections.isEmpty())
	{
		DisplaySide connectionPoint_displaySide = Display::getOppositeSide(whereTo);

		sf::Vector2f connectTo_sidePoint = Display::getSidePointsOfShape(&connectTo->shape, whereTo);
		
		this->connections.addConnection(connectionPoint_displaySide, connectTo);

		//rotate connection point
		Display::rotateShapeByFloat(&this->shape, connectTo->shape.getRotation());

		Display::setOriginForShape(&this->shape, connectionPoint_displaySide);
		this->shape.setPosition(connectTo_sidePoint.x, connectTo_sidePoint.y);

		if (whereTo == FRONT)
			connectTo->frontPoint = this;
		else if (whereTo == BACK)
			connectTo->backPoint = this;
	}
	else if (this->connections.size() != this->maxConnections)
	{
		DisplaySide connectionPoint_displaySide = this->getRandomFreeSide();
		
		if (connectionPoint_displaySide == INVALID_SIDE)
		{
			std::cout << connectTo->name << std::endl;
			std::cout << this << std::endl;
			std::cout << "Cannot add more side " << std::endl;
			std::cout << "------------------------------" << std::endl;
			return;
		}

		Display::rotateShapeBySide(&connectTo->shape, connectionPoint_displaySide);
		Display::rotateShapeForConnection(&connectTo->shape, &this->shape);

		sf::Vector2f connectionPoint_sidePoint = Display::getSidePointsOfShape(&this->shape, connectionPoint_displaySide);

		this->connections.addConnection(connectionPoint_displaySide, connectTo);
		
		Display::setOriginForShape(&connectTo->shape, Display::getOppositeSide(whereTo));
		connectTo->shape.setPosition(connectionPoint_sidePoint.x, connectionPoint_sidePoint.y);
	}
}

Display::DisplaySide Display::ConnectionPoint::getRandomFreeSide()
{
	if (this->connections.isFree(Display::LEFT))
		return Display::LEFT;
	else if (this->connections.isFree(Display::RIGHT))
		return Display::RIGHT;
	else if (this->connections.isFree(Display::FRONT))
		return Display::FRONT;
	else if (this->connections.isFree(Display::BACK))
		return Display::BACK;
	else
		return Display::INVALID_SIDE;
}

Display::DisplaySide Display::ConnectionPoint::getWhereStreetConnected(Street* object)
{
	if (this == nullptr)
		return INVALID_SIDE;

	if (object == connections.left)
		return LEFT;
	else if (object == connections.right)
		return RIGHT;
	else if (object == connections.front)
		return FRONT;
	else if (object == connections.back)
		return BACK;
	else
		return INVALID_SIDE;
}

Display::DisplaySide Display::ConnectionPoint::getWhereToConnectCloseStreets(Street* street)
{
	sf::Vector2f streetBackPoint = Display::getSidePointsOfShape(&street->shape, BACK);

	sf::Vector2f c_pointBackPoint = Display::getSidePointsOfShape(&this->shape, BACK);
	sf::Vector2f c_pointLeftPoint = Display::getSidePointsOfShape(&this->shape, LEFT);
	sf::Vector2f c_pointRightPoint = Display::getSidePointsOfShape(&this->shape, RIGHT);

	if (streetBackPoint == c_pointBackPoint)
		return BACK;
	else if (streetBackPoint == c_pointLeftPoint)
		return LEFT;
	else if (streetBackPoint == c_pointRightPoint)
		return RIGHT;
	else
		return INVALID_SIDE;
}

short Display::getNumberFromSide(DisplaySide side)
{
	if (side == FRONT)
		return 0;
	else if (side == LEFT)
		return 1;
	else if (side == BACK)
		return 2;
	else if (side == RIGHT)
		return 3;
}

Display::DisplaySide Display::convertNumberToDisplaySide(short number)
{
	if (number == -1)
		number = 3;
	else if (number == -2)
		number = 2;
	else if (number == -3)
		number = 1;


	if (number == 0)
		return FRONT;
	else if (number == 1)
		return LEFT;
	else if (number == 2)
		return BACK;
	else if (number == 3)
		return RIGHT;
}

void Display::displayStreet(Street* street, City* city)
{
	street->isDisplayed = true;

	if (street->isRoot)
	{
		street->shape.setTexture(*Assets::getObjectTexture(Assets::rootTextureName));
		street->shape.setPosition(xRootPosition, yRootPosition);
	}
	else // get where to snap it
	{
		street->shape.setTexture(*Assets::getObjectTexture(Assets::streetTextureName));

		Street* parentStreet = CityFunctions::getWhereToConnectStreet(street);

		DisplaySide displaySide = Display::convertVectorSideToDisplaySide(CityFunctions::getWhereToSnap(street, parentStreet));

		ConnectionPoint* parentStreet_ConnectionPoint = Display::getExistingConnectionPoint(parentStreet, displaySide);
		if (parentStreet_ConnectionPoint == nullptr)
			parentStreet_ConnectionPoint = Display::createConnectionPoint();

		parentStreet_ConnectionPoint->connectToShape(street, BACK);
		street->frontPoint = parentStreet_ConnectionPoint;

		ConnectionPoint* closestPoint = Display::checkForClosePoints(city, street, BACK);
		if (closestPoint != nullptr)
		{
			std::cout << "closestpoint to: " << street->name << " is " << closestPoint << std::endl;
			//closestPoint->connectToShape(street, closestPoint->getWhereToConnectCloseStreets(street));

			//street->backPoint = closestPoint;
		}
	}

	Display::createPointsToStreet(street);

	window.draw(street->shape);
}

void Display::displayCity(City* city)
{
	for (auto& str : city->streets)
	{
		if (str->connectedStreetsBack.empty() && str->connectedStreetsFront.empty())
		{
			continue;
		}
		Display::displayStreet(str, city);
	}

	Display::rearragneStreetsByCords(city);

	Display::window.display();
}

void Display::refreshFrame(const std::vector<City*>& citiesDisplay)
{
	window.clear();

	for (auto& city : citiesDisplay)
	{
		for (auto& str : city->streets)
		{
			window.draw(str->shape);
			if (str->frontPoint)
			{
				window.draw(str->frontPoint->shape);
			}
			if (str->backPoint)
			{
				window.draw(str->backPoint->shape);
			}
		}
	}
	window.display();
}

sf::Vector2f Display::getSidePointsOfShape(sf::Sprite* shape, DisplaySide side)
{
	// correcting side according to rotation
	side = Display::convertNumberToDisplaySide(Display::getNumberFromSide(side) - (int)shape->getRotation() / 90.f);

	switch (side)
	{
	case Display::BACK:
	{
		sf::Vector2f botLeft = Display::getBotLeft(shape);
		sf::Vector2f botRight = Display::getBotRight(shape);
		return Display::calculateMiddlePoint(botLeft, botRight);
	}
	case Display::FRONT:
	{
		sf::Vector2f topLeft = Display::getTopLeft(shape);
		sf::Vector2f topRight = Display::getTopRight(shape);
		return Display::calculateMiddlePoint(topLeft, topRight);
	}
	case Display::LEFT:
	{
		sf::Vector2f topLeft = Display::getTopLeft(shape);
		sf::Vector2f botLeft = Display::getBotLeft(shape);
		return Display::calculateMiddlePoint(topLeft, botLeft);
	}
	case Display::RIGHT:
	{
		sf::Vector2f topRight = Display::getTopRight(shape);
		sf::Vector2f botRight = Display::getBotRight(shape);
		return Display::calculateMiddlePoint(topRight, botRight);
	}
	}
}

void Display::setOriginForShape(sf::Sprite* shape, DisplaySide side)
{
	sf::FloatRect bounds = shape->getLocalBounds();
	if (side == LEFT)
		shape->setOrigin(bounds.left, bounds.height / 2);
	else if (side == BACK)
		shape->setOrigin(bounds.width / 2, bounds.height);
	else if (side == RIGHT)
		shape->setOrigin(bounds.width, bounds.height / 2);
	else if (side == FRONT)
		shape->setOrigin(bounds.width / 2, bounds.top);
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

float Display::getSmallerX_Vector2f(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
	if (vec1.x < vec2.x)
	{
		return vec1.x;
	}
	else if (vec1.x > vec2.x)
	{
		return vec2.x;
	}
	else
	{
		return 0;
	}
}

float Display::getSmallerY_Vector2f(const sf::Vector2f& vec1, const sf::Vector2f& vec2)
{
	if (vec1.y < vec2.y)
	{
		return vec1.y;
	}
	else if (vec1.y > vec2.y)
	{
		return vec2.y;
	}
	else
	{
		return 0;
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
	default:
		return Display::INVALID_SIDE;
	}
}

Display::ConnectionPoint* Display::getExistingConnectionPoint(Street* street, DisplaySide side)
{
	if (street->frontPoint && side == FRONT)
	{
		return street->frontPoint;
	}
	else if (street->backPoint && side == BACK)
	{
		return street->backPoint;
	}
	return nullptr;
}

Display::ConnectionPoint* Display::checkForClosePoints(City* city, Street* street, DisplaySide side)
{
	sf::Vector2f streetBackPoint = Display::getSidePointsOfShape(&street->shape, Display::DisplaySide::BACK);
	sf::Vector2f streetFrontPoint = Display::getSidePointsOfShape(&street->shape, Display::DisplaySide::FRONT);

	for (const auto& str : city->streets)
	{
		if (str->backPoint != nullptr && side == BACK && (str->backPoint->shape.getGlobalBounds().contains(streetBackPoint) || str->backPoint->shape.getGlobalBounds().contains(streetFrontPoint)))
		{
			return str->backPoint;
		}
		if (str->frontPoint != nullptr && side == FRONT && (str->frontPoint->shape.getGlobalBounds().contains(streetBackPoint) || str->frontPoint->shape.getGlobalBounds().contains(streetFrontPoint)))
		{
			return str->frontPoint;
		}
	}
	return nullptr;
}

void Display::createPointsToStreet(Street* street)
{
	if (street->isRoot)
	{
		street->frontPoint = Display::createConnectionPoint();
		street->backPoint = Display::createConnectionPoint();
		Display::rotateShapeByFloat(&street->frontPoint->shape, street->shape.getRotation());
		Display::rotateShapeByFloat(&street->backPoint->shape, street->shape.getRotation());

		street->frontPoint->connectToShape(street, Display::FRONT);
		street->backPoint->connectToShape(street, Display::BACK);
		
		Display::window.draw(street->frontPoint->shape);
		Display::window.draw(street->backPoint->shape);
	}
	else if (street->backPoint == nullptr)
	{
		street->backPoint = Display::createConnectionPoint();
		Display::rotateShapeByFloat(&street->backPoint->shape, street->shape.getRotation());

		street->backPoint->connectToShape(street, Display::BACK);
		
		Display::window.draw(street->backPoint->shape);
	}
	else if (street->frontPoint == nullptr)
	{
		street->frontPoint = Display::createConnectionPoint();
		Display::rotateShapeByFloat(&street->frontPoint->shape, street->shape.getRotation());

		street->frontPoint->connectToShape(street, Display::FRONT);
		
		Display::window.draw(street->frontPoint->shape);
	}
}

Display::ConnectionPoint* Display::createConnectionPoint()
{
	ConnectionPoint* res = new ConnectionPoint();
	res->shape.setTexture(*Assets::getObjectTexture(Assets::pointTextureName));
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

sf::Vector2f Display::calculateMiddlePoint(const sf::Vector2f& point1, const sf::Vector2f& point2)
{
	sf::Vector2f smaller = sf::Vector2f(Display::getSmallerX_Vector2f(point1, point2), Display::getSmallerY_Vector2f(point1, point2));

	smaller.x += abs(point1.x - point2.x) / 2.f;
	smaller.y += abs(point1.y - point2.y) / 2.f;

	if (point1.x == point2.x)
	{
		smaller.x = point1.x;
	}
	if (point1.y == point2.y)
	{
		smaller.y = point1.y;
	}

	return smaller;
}

void Display::rotateShapeBySide(sf::Sprite* shape, DisplaySide side)
{
	if (side == Display::LEFT)
		shape->setRotation(90.f);
	else if(side == Display::FRONT)
		shape->setRotation(180.f);
	else if (side == Display::RIGHT)
		shape->setRotation(270.f);
	else
		shape->setRotation(0.f);
}

void Display::rotateShapeByFloat(sf::Sprite* shape, const float& amount)
{
	shape->setRotation(amount);
}

void Display::rotateShapeForConnection(sf::Sprite* shape, sf::Sprite* connectTo)
{
	shape->rotate(connectTo->getRotation());
}

void Display::rearragneStreetsByCords(City* city)
{
	std::vector<Street*> holderForStreetsBack{};
	std::vector<Street*> holderForStreetsFront{};

	for (const auto& str1 : city->streets)
	{
		for (const auto& str2 : city->streets)
		{
			if (str1 == str2)
				continue;
			else if (str1->shape.getGlobalBounds().contains(str2->shape.getPosition()))
			{
				str2->removeStreetFromNeighbors(holderForStreetsBack, holderForStreetsFront);
				city->connectToRandomUnfinishedStreet(str2);
				str1->addStreetToNewNeighbors(holderForStreetsBack, holderForStreetsFront);

				holderForStreetsBack.clear();
				holderForStreetsFront.clear();
			}
		}
	}
	Display::displayNotDisplayed(city);

	std::cout << Display::printSameStreetCords(city) << std::endl;
}

void Display::displayNotDisplayed(City* city)
{
	for (auto& str : city->streets)
	{
		if (!str->isDisplayed)
			Display::displayStreet(str, city);
	}
}

int Display::printSameStreetCords(City* city)
{
	int index = 0;

	for (const auto& str1 : city->streets)
	{
		for (const auto& str2 : city->streets)
		{
			if (str1 == str2)
				continue;
			else if (str1->shape.getGlobalBounds().contains(str2->shape.getPosition()))
			{
				std::cout << "pos match at: x: " << str1->shape.getPosition().x << " y: " << str1->shape.getPosition().y << " names: " << str1->name << " " << str2->name << std::endl;
				index++;
				break;
			}
		}
	}
	return index;
}

std::string Display::sideToString(DisplaySide side)
{
	if (side == FRONT)
		return "front";
	else if (side == BACK)
		return "back";
	else if (side == RIGHT)
		return "right";
	else if (side == LEFT)
		return "left";
	else if (side == INVALID_SIDE)
		return "INVALID_SIDE";
	else
		return "none";
}
