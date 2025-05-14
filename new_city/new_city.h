#ifndef NEW_CITY_H
#define NEW_CITY_H

#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class Street
{
private:
    char name[20];
    std::vector<Street*> connectedStreetsBack;
    std::vector<Street*> connectedStreetsFront;

public:
    uint8_t maxBackStreetsSize = 3; 
    uint8_t maxFrontStreetsSize = 3; 
    bool frontFull = false;
    bool backFull = false;

    Street() = default;
    Street(char name[]);
    Street(char name[], std::vector<Street*>& back, std::vector<Street*>& front);

    void addStreetToBack(Street* street);
    void addStreetToFront(Street* street);

    int getBackStreetsCount();
    int getFrontStreetsCount();

    void changeName(char newName[]);
};

class City
{
private:
    char name[20];
    std::vector<Street*> streets;
    std::vector<Street*> unfinishedStreets;

public:
    City() = default;
    City(char name[]);
    City(char name[], std::vector<Street*> streets);

    int getStreetsCount();
    void changeName(char name[]);
    
    void addStreet(Street* street);
    void addUnfinishedStreet(Street* street);

    Street* getFirstUnfinishedStreet();
};

namespace CityFunctions
{
    char* getRandomName(std::vector<char[]>& names);
    std::vector<char[]> getRandomNamesFromFile();
    City* generateCity(char name[], int streetCount, City* city);
    Street* createRootStreet(char name[]);
    Street* connectStreetsToStreet(Street* target, City* city, std::vector<char[]> randomNames);
}

#endif