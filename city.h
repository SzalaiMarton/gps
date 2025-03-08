#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "funcForDev.cpp"

using namespace std;

class CityParts
{
public:
    const static int DEAFAULT_POPULATION_SIZE = 0;
    const static int LIGHT_POPULATION_SIZE = 20;
    const static int MEDIUM_POPULATION_SIZE = 50;
    const static int HEAVY_POPULATION_SIZE = 100;
    
    enum Density
    {
        LIGHT = LIGHT_POPULATION_SIZE,
        MEDIUM = MEDIUM_POPULATION_SIZE,
        HEAVY = HEAVY_POPULATION_SIZE
    };

    struct Buildings
    {
        string buildingName;
    };

    struct Street
    {
        vector<Buildings> buildings;
        string streetName;
        string city;
        int length;
        vector<Street*> connectedStreetsFront;
        vector<Street*> connectedStreetsBack;

        Street(vector<string> properties);
        
        void attachToStreet(Street*, string);
    };

    struct City
    {
        string cityName;
        vector<Street*> streets;
        int populationInNumber;
        Density populationInDensity;

        City(vector<string>);
        
        void printPopulation();
        void convertPopNumberToDensity();
        void listStreets();
    };

    static void createStreet(vector<string>);
    static void createCity(vector<string>);

    static bool checkForExistingObject(string, string);

    static vector<CityParts::City> cityVector;
    static vector<CityParts::Street> streetVector;
};