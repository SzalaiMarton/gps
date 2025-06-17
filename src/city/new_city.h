#pragma once

#include <vector>
#include <string>

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

class ConnectionPoint
{
public: 
    std::string name;
    uint8_t maxConnection;  // between 1-3
    uint8_t weight;         // between 1-9
    uint8_t cost;
    bool isVisited;
    std::vector<Street*> connections;

    ConnectionPoint();

    ~ConnectionPoint() = default;

    void removeConnection(Street* street);
    bool isStreetConnected(Street* street);
    void calculateCost(uint8_t streetWeight);
    void connectStreet(Street* street, Sides side);
};

class Street
{
public:
    std::string name;
    uint8_t weight;         // between 1-9
    ConnectionPoint* backPoint;
    ConnectionPoint* frontPoint;

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
    void addPoint(ConnectionPoint* point);
    ConnectionPoint* getPointByName(const std::string& name);
    ConnectionPoint* getRandomUnfinishedPoint(ConnectionPoint* targetPoint);
    void connectPointToRandomPoint(ConnectionPoint* point);
    std::vector<ConnectionPoint*> getShortestPath(const std::string& base, const std::string& destination);
    bool isPointExist(const std::string& name);
    void flipVisited();

    void printPoints(bool details);
    void printStreets(bool details);
};

