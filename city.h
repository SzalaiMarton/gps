#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "funcForDev.cpp"
#include <limits>

using namespace std;

class CityParts
{
public:
    struct Buildings
    {
        string buildingName;
    };

    struct Street
    {
        string streetName = "default";
        string city = "None";
        vector<Buildings> buildings;
        vector<Street*> connectedStreetsFront;
        vector<Street*> connectedStreetsBack;

        Street(vector<string>);
        
        void attachStreet(Street*, string);
        void listProperties();
        void listBuildings();
        void listConnectedStreetsBack();
        void listConnectedStreetsFront();

        void editName();
        void editCity();
        void editBuildings(string, CityParts::Buildings);
        void editConnectedStreetsBack(string, CityParts::Street*);
        void editConnectedStreetsFront(string, CityParts::Street*);

        void deleteCity();
    };

    struct City
    {
        string cityName = "default";
        vector<Street*> streets;
        
        City(vector<string>);
        
        void listStreets();
        void listProperties();

        void editName();
        void editStreets(string);
    };

    static void createStreet(vector<string>);
    static void createCity(vector<string>);
    static Street* generateRandomStreet(string, vector<string>&);
    static vector<string> randomNameGenerator();
    static void generateRandomCity(string, int, int);

    static bool checkForExistingObject(string, string);
    static bool checkIfStreetInCityAlready(string, string);

    static vector<CityParts::City> cityVector;
    static vector<CityParts::Street> streetVector;

    static int getCityIndex(string);
};