#pragma once

#include "../city/new_city.h"
#include <map>

namespace Display
{
	enum DisplaySide
	{
		BACK,
		FRONT,
		LEFT,
		RIGHT,
		INVALID_SIDE
	};

	template <typename T>
	class ConnectionsToShape
	{
	public:
		ConnectionsToShape() = default;
		ConnectionsToShape(std::map<DisplaySide, T> elementsToBeAdded)
		{
			for (const auto& [side, element] : elementsToBeAdded)
			{
				this->addConnection(side, element);
			}
		}
		ConnectionsToShape(T* elements[4])
		{
			for (short index = 0; index < 4; index++)
			{
				try
				{
					this->addWithoutSide(elements[index]);
				}
				catch (const std::exception&)
				{
					return;
				}
			}
		}
		void addConnection(DisplaySide side, T element)
		{
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
			else if (side == Display::RIGHT && left == nullptr)
			{
				return true;
			}
			else if (side == Display::FRONT && left == nullptr)
			{
				return true;
			}
			else if (side == Display::BACK && left == nullptr)
			{
				return true;
			}
			return false;
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

		ConnectionPoint() = default;
		void connectToShape(Street* connectTo, DisplaySide side);
		DisplaySide getRandomFreeSide();
	};

	extern int width;
	extern int height;
	extern sf::RenderWindow window;

	extern float xRootPosition, yRootPosition;
	
	void displayStreet(Street* street, Street* connectTo);
	void displayCity(City* city);
	void refreshFrame(const std::vector<City*>& citiesDisplay);

	sf::Vector2f* getSidePoints(sf::Sprite* shape, DisplaySide side);
	sf::Vector2f getTopRight(sf::Sprite* shape);
	sf::Vector2f getBotRight(sf::Sprite* shape);
	sf::Vector2f getBotLeft(sf::Sprite* shape);
	sf::Vector2f getTopLeft(sf::Sprite* shape);
	float* getSmallerX_Vector2f(sf::Vector2f* vec1, sf::Vector2f* vec2);
	float* getSmallerY_Vector2f(sf::Vector2f* vec1, sf::Vector2f* vec2);

	DisplaySide getOppositeSide(DisplaySide side);

	void createPointsToStreet(Street* street);
	ConnectionPoint* createConnectionPoint();
	DisplaySide convertVectorSideToDisplaySide(CityFunctions::VectorSide side);
}
