#ifndef CITY_H
#define CITY_H

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

        Street(const string& properties);
        
        void attachStreet(Street* streetToAttach, const string& side);
        void listProperties();
        void listBuildings();
        void listConnectedStreetsBack();
        void listConnectedStreetsFront();

        void editName();
        void editCity();
        void editBuildings(const string& action, const CityParts::Buildings& building);
        void editConnectedStreetsBack(const string& action, CityParts::Street* street);
        void editConnectedStreetsFront(const string& action, CityParts::Street* street);

        void deleteCity();
    };

    struct City
    {
        string cityName = "default";
        vector<Street*> streets;
        
        City(string& properties);
        
        void listStreets();
        void listProperties();

        void editName();
        void editStreets(const string& action);
    };

    static Street* createStreet(string& properties);
    static void createCity(string& properties);

    static string& pickRandomName(vector<string>& nameList);
    static vector<string> randomNameGenerator();
    static void generateCityForWeb(string& cityName, int streetCount, int maxConnections);
    static void generateStreet(Street* rootStreet, vector<Street*>& unfinishedStreets, vector<string>& randomNames, const string& side);

    static bool checkForExistingObject(const string& name, const string& objectType);
    static bool checkIfStreetInCityAlready(const string& streetName, const string& cityName);

    static vector<CityParts::City> cityVector;
    static vector<CityParts::Street> streetVector;

    static int getCityIndex(const string& cityName);
};

#endif