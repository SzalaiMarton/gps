#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "funcForDev.cpp"

using namespace std;

class CityParts
{
public:
    const static int LIGHT_POPULATION_SIZE = 2000;
    const static int MEDIUM_POPULATION_SIZE = 50000;
    const static int HEAVY_POPULATION_SIZE = 200000;
    
    enum Density
    {
        LIGHT = LIGHT_POPULATION_SIZE,
        MEDIUM = MEDIUM_POPULATION_SIZE,
        HEAVY = HEAVY_POPULATION_SIZE
    };

    class DensityUtils
    {
    public:
        static string toString(Density density)
        {
            switch (density)
            {
            case LIGHT:
                return "Light";
            case MEDIUM:
                return "Medium";
            case HEAVY:
                return "Heavy";
            }
        }
    };

    struct Buildings
    {
        string buildingName;
    };

    struct Street
    {
        string streetName;
        string city;
        int length;
        vector<Buildings> buildings;
        vector<Street*> connectedStreetsFront;
        vector<Street*> connectedStreetsBack;

        Street();
        Street(vector<string>);
        
        void attachToStreet(Street*, string);
        void listProperties();
        void listBuildings();
        void listConnectedStreetsBack();
        void listConnectedStreetsFront();
    };

    struct City
    {
        string cityName;
        int populationInNumber;
        Density populationInDensity;
        vector<Street*> streets;
        
        City(vector<string>);
        
        void convertPopNumberToDensity();
        void listStreets();
        void listProperties();

        void editName(string);
        void editPopulation(int);
        void editStreets();
    };

    static void createStreet(vector<string>);
    static void createCity(vector<string>);

    static bool checkForExistingObject(string, string);

    static vector<CityParts::City> cityVector;
    static vector<CityParts::Street> streetVector;
};