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

        Street(const vector<string>&);
        
        void attachStreet(Street*, const string& side);
        void listProperties();
        void listBuildings();
        void listConnectedStreetsBack();
        void listConnectedStreetsFront();

        void editName();
        void editCity();
        void editBuildings(const string&, const CityParts::Buildings&);
        void editConnectedStreetsBack(const string&, CityParts::Street*);
        void editConnectedStreetsFront(const string&, CityParts::Street*);

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
        void editStreets(const string&);
    };

    static Street* createStreet(const vector<string>&);
    static void createCity(const vector<string>&);

    static string pickRandomName(vector<string>&);
    static vector<string> randomNameGenerator();
    static void generateCityForWeb(const string&, int, int);
    static void generateStreet(Street*, vector<Street*>&, vector<string>&, const string&);

    static bool checkForExistingObject(const string&, const string&);
    static bool checkIfStreetInCityAlready(const string&, const string&);

    static vector<CityParts::City> cityVector;
    static vector<CityParts::Street> streetVector;

    static int getCityIndex(const string&);
};

#endif