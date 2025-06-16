#pragma once

#include <vector>
#include <string>

class Street;
class City;
class ConnectionPoint;

namespace CityFunctions
{
    enum VectorSide
    {
        VECTOR_FRONT,
        VECTOR_BACK,
        VECTOR_INVALID_SIDE
    };

    std::string vectorSideToString(VectorSide side);

    std::string getRandomName(std::vector<std::string>& names);
    std::vector<std::string> getRandomNamesFromFile();
    City* generateCity(const std::string& name, int maxStreetCount);
    Street* createRootStreet(const std::string& name, City* city);
};

class ConnectionPoint
{
public: 
    uint8_t weight;
    std::vector<Street*> connectedTo;

    ConnectionPoint(Street* street);

    ~ConnectionPoint() = default;

    void removeConnection(Street* street);
    bool isStreetConnected(Street* street);
};

class Street
{
public:
    std::string name;
    std::vector<Street*> connectedStreetsFront;
    std::vector<Street*> connectedStreetsBack;
    uint8_t idealConnectionSize = 3;
    uint8_t weight;
    bool isFinished;
    bool isRoot;
    ConnectionPoint* backPoint;
    ConnectionPoint* frontPoint;

    Street() = default;
    Street(const std::string& name);

    ~Street();

    bool addStreetToBack(Street* street, bool addToTheOther);
    bool addStreetToFront(Street* street, bool addToTheOther);

    void createPointToStreet();

    bool isStreetConnected(Street* street);
    CityFunctions::VectorSide getEmptyVectorSide() const;
};

class City
{
public:
    std::string name;
    std::vector<Street*> unfinishedStreets; 
    Street* rootStreet;
    std::vector<Street*> streets;
    std::vector<ConnectionPoint*> points;

    City() = default;
    City(const std::string& name);

    ~City();

    int getUnfinishedCount() const;
    Street* getFirstUnfinishedStreet();
    Street* getMiddleUnfinishedStreet();

    void addStreet(Street* street);
    void addUnfinishedStreet(Street* street);
    void removeFinishedFromUnfinished();
    void connectStreets();
    void connectToRandomUnfinishedStreet(Street* street);

    void getShortestRoute(const std::string& from, const std::string& to, City* city);

    void printAllStreetReferenceCount();

    void printStreets(bool printConnected);
    void printUnfinished();
};

