#pragma once

#include "../city/new_city.h"
#include <map>

namespace Display
{
	enum DisplaySide
	{
		FRONT = 0,
		LEFT = 1,
		BACK = 2,
		RIGHT = 3,
		INVALID_SIDE = 5
	};

	short getNumberFromSide(DisplaySide side);
	DisplaySide convertNumberToDisplaySide(short number);

	template <typename T>
	class ConnectionsToShape
	{
	public:
		ConnectionsToShape()
		{
			left = nullptr;
			right = nullptr;
			back = nullptr;
			front = nullptr;
		}
		void addConnection(DisplaySide side, T element)
		{
			if (this->isEmpty())
			{
				this->front = element;
				return;
			}

			if (side == Display::BACK)
				this->back = element;
			else if (side == Display::FRONT)
				this->front = element;
			else if (side == Display::RIGHT)
				this->right = element;
			else if (side == Display::LEFT)
				this->left = element;
		}
		bool addWithoutSide(T element)
		{
			if (this->left != nullptr)
			{
				this->left = element;
				return true;
			}
			else if (this->right != nullptr)
			{
				this->right = element;
				return true;
			}
			else if (this->front != nullptr)
			{
				this->front = element;
				return true;
			}
			else if (this->back != nullptr)
			{
				this->back = element;
				return true;
			}
			else
			{
				return false;
			}
		}
		bool isEmpty()
		{
			if (this->left == nullptr && this->right == nullptr && this->front == nullptr && this->back == nullptr)
				return true;
			else
				return false;
		}
		bool isFree(DisplaySide side)
		{
			if (side == Display::LEFT && left == nullptr)
			{
				return true;
			}
			else if (side == Display::RIGHT && right == nullptr)
			{
				return true;
			}
			else if (side == Display::FRONT && front == nullptr)
			{
				return true;
			}
			else if (side == Display::BACK && back == nullptr)
			{
				return true;
			}
			return false;
		}
		uint8_t size()
		{
			uint8_t size = 0;
			if (!left)
				size++;
			if (!right)
				size++;
			if (!front)
				size++;
			if (!back)
				size++;
			return size;
		}
		void printValues()
		{
			std::cout << "left: " << left << " right: " << right << " \nfront: " << front << " back: " << back << std::endl;
			std::cout << "-----------------------------------------------------" << std::endl;
		}
		

		T left;
		T right;
		T back;
		T front;
	};

	class ConnectionPoint
	{
	public:
		sf::Sprite shape;
		ConnectionsToShape<Street*> connections;
		uint8_t maxConnections = 4;

		ConnectionPoint();
		void connectToShape(Street* connectTo, DisplaySide whereTo);
		DisplaySide getRandomFreeSide();
		DisplaySide getWhereStreetConnected(Street* street);
		DisplaySide getWhereToConnectCloseStreets(Street* street);
	};

	extern int width;
	extern int height;
	extern sf::RenderWindow window;
	extern float xRootPosition, yRootPosition;

	void displayStreet(Street* street, City* city);
	void displayCity(City* city);
	void refreshFrame(const std::vector<City*>& citiesDisplay);

	sf::Vector2f getSidePointsOfShape(sf::Sprite* shape, DisplaySide side);
	void setOriginForShape(sf::Sprite* shape, DisplaySide side);

	sf::Vector2f getTopLeft(sf::Sprite* shape);
	sf::Vector2f getTopRight(sf::Sprite* shape);
	sf::Vector2f getBotLeft(sf::Sprite* shape);
	sf::Vector2f getBotRight(sf::Sprite* shape);

	float getSmallerX_Vector2f(const sf::Vector2f& vec1, const sf::Vector2f& vec2);
	float getSmallerY_Vector2f(const sf::Vector2f& vec1, const sf::Vector2f& vec2);

	DisplaySide getOppositeSide(DisplaySide side);

	ConnectionPoint* getExistingConnectionPoint(Street* street, DisplaySide side);
	ConnectionPoint* checkForClosePoints(City* city, Street* street, DisplaySide side);

	void createPointsToStreet(Street* street);
	ConnectionPoint* createConnectionPoint();
	DisplaySide convertVectorSideToDisplaySide(CityFunctions::VectorSide side);

	sf::Vector2f calculateMiddlePoint(const sf::Vector2f& point1, const sf::Vector2f& point2);

	void rotateShapeBySide(sf::Sprite* shape, DisplaySide side);
	void rotateShapeByFloat(sf::Sprite* shape, const float& customRotation);
	void rotateShapeForConnection(sf::Sprite* shape, sf::Sprite* connectTo);

	void rearragneStreetsByCords(City* city);

	void displayNotDisplayed(City* city);

	int printSameStreetCords(City* city);
	std::string sideToString(DisplaySide side);
}
