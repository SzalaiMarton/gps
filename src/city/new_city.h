#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <random>
#include "../../include/SFML/Graphics.hpp"

namespace Display {
    class ConnectionPoint;
}

class Street;
class City;

namespace CityFunctions
{
    enum VectorSide
    {
        VECTOR_FRONT,
        VECTOR_BACK,
        VECTOR_INVALID_SIDE
    };

    std::string vectorSideToString(VectorSide side);

    Street* getWhereToConnectStreet(Street* street);
    std::string getRandomName(std::vector<std::string>& names);
    std::vector<std::string> getRandomNamesFromFile();
    City* generateCity(const std::string& name, int maxStreetCount);
    Street* createRootStreet(const std::string& name, City* city);
    VectorSide getWhereToSnap(Street* target, Street* connectTo);
};

class Street
{
public:
    std::string name;
    std::vector<Street*> connectedStreetsFront;
    std::vector<Street*> connectedStreetsBack;
    uint8_t maxBackStreetsSize = 3;
    uint8_t maxFrontStreetsSize = 3; 
    bool isFrontFull;
    bool isBackFull;
    bool isFinished;
    bool isDisplayed;
    bool isRoot;
    sf::Sprite shape;
    Display::ConnectionPoint* frontPoint;
    Display::ConnectionPoint* backPoint;

    Street() = default;
    Street(const std::string& name);
    Street(const std::string& name, std::vector<Street*>& back, std::vector<Street*>& front);

    void checkIfBackFull();
    void checkIfFrontFull();
    
    void addStreetToBack(Street* street, bool addToTheOther);
    void addStreetToFront(Street* street, bool addToTheOther);
    void addStreetToNewNeighbors(std::vector<Street*>& holderForStreetsBack, std::vector<Street*>& holderForStreetsFront);

    void removeFromBack(Street* street, bool removeTheOther);
    void removeFromFront(Street* street, bool removeTheOther);
    void removeStreetFromNeighbors(std::vector<Street*>& holderForStreetsBack, std::vector<Street*>& holderForStreetsFront);

    void changeTexture(sf::Texture* texture);
    bool isStreetConnected(Street* street);
    void changeName(const std::string& newName);
    CityFunctions::VectorSide getEmptyVectorSide();
};

class City
{
public:
    std::string name;
    std::vector<Street*> unfinishedStreets; sf::Sprite shape;
    Street* rootStreet;
    std::vector<Street*> streets;


    City() = default;
    City(const std::string& name);
    City(const std::string& name, std::vector<Street*> streets);

    int getUnfinishedCount();
    Street* getFirstUnfinishedStreet();
    Street* getMiddleUnfinishedStreet();
    void changeName(const std::string& name);
    void changeTexture(sf::Texture* texture);

    void addStreet(Street* street);
    void addUnfinishedStreet(Street* street);
    void removeUnfinishedStreet(Street* street);
    void connectStreets();
    void connectToRandomUnfinishedStreet(Street* street);

    void printStreets(bool printConnected);
    void printUnfinished();
};

