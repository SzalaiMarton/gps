#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <cstdint>
#include <random>
#include "../../include/SFML/Graphics.hpp"

class Street
{
private:
    std::string name;
    std::vector<Street*> connectedStreetsBack;
    std::vector<Street*> connectedStreetsFront;

public:
    uint8_t maxBackStreetsSize = 3; 
    uint8_t maxFrontStreetsSize = 3; 
    bool frontFull;
    bool backFull;
    bool isDisplayed;
    bool isConnectedToPoint_Front;
    bool isConnectedToPoint_Back;
    bool root;
    sf::Sprite shape;
    Display::ConnectionPoint* frontPoint;
    Display::ConnectionPoint* backPoint;

    Street() = default;
    Street(const std::string& name);
    Street(const std::string& name, std::vector<Street*>& back, std::vector<Street*>& front);

    void addStreetToBack(Street* street);
    void addStreetToFront(Street* street);
    void changeTexture(sf::Texture* texture);

    int getBackStreetsCount();
    int getFrontStreetsCount();
    std::vector<Street*> getConnectedStreetsBack();
    std::vector<Street*> getConnectedStreetsFront();
    std::string getName();
    

    void changeName(const std::string& newName);
};

class City
{
private:
    std::string name;
    std::vector<Street*> unfinishedStreets;

public:
    sf::Sprite shape;
    Street* rootStreet;
    std::vector<Street*> streets;


    City() = default;
    City(const std::string& name);
    City(const std::string& name, std::vector<Street*> streets);

    int getStreetsCount();
    int getUnfinishedCount();
    std::vector<Street*> getStreets();
    void changeName(const std::string& name);
    void changeTexture(sf::Texture* texture);

    void addStreet(Street* street);
    void addUnfinishedStreet(Street* street);
    void removeUnfinishedStreet(Street* street);

    Street* getFirstUnfinishedStreet();
    void printStreets(bool printConnected);
};

namespace CityFunctions
{
    enum VectorSide
    {
        VECTOR_FRONT,
        VECTOR_BACK,
        VECTOR_INVALID_SIDE
    };

    std::string getRandomName(std::vector<std::string>& names);
    std::vector<std::string> getRandomNamesFromFile();
    City* generateCity(const std::string& name, int maxStreetCount);
    Street* createRootStreet(const std::string& name, City* city);
    bool connectStreetsToStreet(Street* target, City* city, std::vector<std::string> randomNames, int maxStreetCount);
    VectorSide getWhereToSnap(Street* target, Street* connectTo);
};
