#pragma once

#include <vector>
#include <string>
#include "../../include/SFML/Graphics.hpp"

#define LOGGING 0
#define DEBUGGING 0

#define FORCE_LOG(s) std::cout << s << std::endl

// DEBUG LOGGING
#if DEBUGGING
#define DEBUG_LOG(s) std::cout << s << std::endl
#else
#define DEBUG_LOG(s) ((void)0)
#endif

// LOGGING
#if LOGGING
#define LOG(s) std::cout << s << std::endl
#else
#define LOG(s) ((void)0)
#endif

class Street;
class City;
class ConnectionPoint;

namespace CityFunctions
{
    extern std::vector<std::string> pointNames;
    extern std::vector<std::string> streetNames;
    extern std::vector<ConnectionPoint*> candidatePoints;
    extern std::vector<sf::Sprite*> route;

    City* generateCity(const std::string& name, int pointCount);
    std::string getRandomName(std::vector<std::string>* names);
    std::vector<std::string> readFile(const std::string& path);
    void disconnectTwoPoints(ConnectionPoint* point1, ConnectionPoint* point2, City* city);
    bool connectTwoPoints(ConnectionPoint* point1, ConnectionPoint* point2, City* city, bool& outOfCandidates);
    ConnectionPoint* getValidPoint(City* city, ConnectionPoint* dontMatch, bool checkIsFull, bool checkNoSpaceAround);
    ConnectionPoint* getRandomCandidate(ConnectionPoint* dontMatch);
    void removeFromCandidates(ConnectionPoint* point);
    Street* getSharedStreet(ConnectionPoint* point1, ConnectionPoint* point2);
    void clearRoute();
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
    sf::Sprite* shape;

    bool isDisplayed() const;
};

class ConnectionPoint : public Object
{
public: 
    uint8_t maxConnection;  // between 2-3
    uint8_t cost;
    bool isVisited;
    bool noSpaceAround;
    std::vector<Street*> connectedStreets;
    std::vector<ConnectionPoint*> connectedPoints;

    ConnectionPoint();

    ~ConnectionPoint();

    bool addStreet(Street* street);
    void removeStreet(Street* street);

    bool addPoint(ConnectionPoint* point, bool& isAlreadyIn);
    void removePoint(ConnectionPoint* point);

    bool isStreetConnected(Street* street);
    bool isPointAlreadyConnected(ConnectionPoint* point);
    bool isFull() const;

    void calculateCost(uint8_t streetWeight);
    Street* getSharedStreet(ConnectionPoint* point);
};

class Street : public Object
{
public:
    ConnectionPoint* backPoint;
    ConnectionPoint* frontPoint;
    bool goFront;
    bool goBack;

    int appeared;

    Street();

    ~Street();
    
    bool removePoint(ConnectionPoint* point);
    void addPoint(ConnectionPoint* point, Sides side);
};

class City
{
public:
    std::string name;
    std::vector<Street*> streets;
    std::vector<ConnectionPoint*> points;
    std::vector<ConnectionPoint*> toBeDisplayedPoints;
    ConnectionPoint* firstDisplayPoint;

    City() = default;
    City(const std::string& name);

    ~City();

    void addStreet(Street* street);
    void deleteStreet(Street* street);
    void removeUnusedStreets();
    void addPoint(ConnectionPoint* point, bool isDisplayed);
    bool isPointExist(const std::string& name);

    ConnectionPoint* getPointByName(const std::string& name);
    ConnectionPoint* getRandomPoint(int beginIndex = -1);
    ConnectionPoint* getLastCreatedPoint();

    std::vector<ConnectionPoint*> getShortestPath(ConnectionPoint* base, ConnectionPoint* destination);
    void flipVisited();
    void turnStreetsIntoLines();
    void turnStreetsIntoVectors(ConnectionPoint* destination);
    bool isValidConnectionPoint(ConnectionPoint* point);

    void countMoreThan2ConnectionStreets();
    void countDisplayedPoints();
    void printPoints(bool details, bool relatedPoints);
    void printStreets(bool details);
};