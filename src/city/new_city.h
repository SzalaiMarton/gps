#pragma once

#include <vector>
#include <string>
#include "../../include/SFML/Graphics.hpp"

class Street;
class City;
class ConnectionPoint;

namespace CityFunctions
{

    extern std::vector<std::string> pointNames;
    extern std::vector<std::string> streetNames;

    City* generateCity(const std::string& name, int pointCount);
    std::string getRandomName(std::vector<std::string>& names);
    std::vector<std::string> readFile(const std::string& path);
};

enum Sides
{
    FRONT,
    BACK
};

class Object
{
public:
    Object() = default;
    virtual ~Object() = default;
    
    std::string name;
    uint8_t weight;         // between 1-9
    bool isDisplayed;
    sf::Sprite* shape;
};

class ConnectionPoint : public Object
{
public: 
    uint8_t maxConnection;  // between 2-3
    uint8_t cost;
    bool isVisited;
    std::vector<Street*> connections;

    ConnectionPoint();

    ~ConnectionPoint() = default;

    void removeRelatedPoint(ConnectionPoint* toBeRemoved, City* city);
    void removeConnection(Street* street);
    bool isStreetConnected(Street* street);
    void calculateCost(uint8_t streetWeight);
    void connectStreet(Street* street, Sides side);
    void connectPoint(ConnectionPoint* point, City* city);
    bool isPointAlreadyConnected(ConnectionPoint* point);
    Street* getSharedStreet(ConnectionPoint* point);
};

class Street : public Object
{
public:
    ConnectionPoint* backPoint;
    ConnectionPoint* frontPoint;
    bool goFront;
    bool goBack;

    Street();

    ~Street();
};

class City
{
public:
    std::string name;
    std::vector<Street*> streets;
    std::vector<ConnectionPoint*> points;

    City() = default;
    City(const std::string& name);

    ~City();

    void addStreet(Street* street);
    void removeStreet(Street* street);
    void addPoint(ConnectionPoint* point);
    ConnectionPoint* getPointByName(const std::string& name);
    ConnectionPoint* getRandomUnfinishedPoint(ConnectionPoint* targetPoint);
    ConnectionPoint* getRandomPoint();
    ConnectionPoint* getLastCreatedPoint();
    void connectPointToRandomPoint(ConnectionPoint* point);
    std::vector<ConnectionPoint*> getShortestPath(const std::string& base, const std::string& destination);
    bool isPointExist(const std::string& name);
    void flipVisited();
    void turnStreetsIntoLines();
    void turnStreetsIntoVectors(ConnectionPoint* destination);
    void movePointToTheBack(ConnectionPoint* point);

    void printPoints(bool details, bool relatedPoints);
    void printStreets(bool details);
};

