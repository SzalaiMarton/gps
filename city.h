#pragma once

#include <string>
#include <vector>
#include <iostream>

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
        string building_name;
    };

    struct Street
    {
        vector<Buildings> buildings;
        string street_name;
        int length;
        vector<Street*> connected_streets_front;
        vector<Street*> connected_streets_back;
        Street(string name, int street_length)
        {
            street_name = name;
            length = street_length;
        }

        void attachToStreet(Street* rootStreet, string side)
        {
            if(side == "back")
            {
                rootStreet->connected_streets_back.push_back(this);
            }
            else if(side == "front")
            {
                rootStreet->connected_streets_front.push_back(this);
            }
        }
    };

    struct City
    {
        string cityName;
        vector<Street*> streets;
        int populationInNumber;
        Density populationInDensity;
        City(string name, int pop)
        {
            cityName = name;
            populationInNumber = pop;
            convertPopNumberToDensity();
        }

        void printPopulation()
        {
            cout << cityName + "'s population: " + to_string(streets.size());
        }

        void convertPopNumberToDensity()
        {
            if(populationInNumber <= CityParts::LIGHT_POPULATION_SIZE)
            {
                populationInDensity = CityParts::Density::LIGHT;
            }
            else if(populationInNumber > CityParts::LIGHT_POPULATION_SIZE && populationInNumber <= CityParts::MEDIUM_POPULATION_SIZE)
            {
                populationInDensity = CityParts::Density::MEDIUM;
            }
            else if(populationInNumber > CityParts::MEDIUM_POPULATION_SIZE && populationInNumber <= CityParts::HEAVY_POPULATION_SIZE)
            {
                populationInDensity = CityParts::Density::HEAVY;
            }
            else if(populationInNumber > CityParts::HEAVY_POPULATION_SIZE)
            {
                populationInDensity = CityParts::Density::HEAVY;
            }
        }

        void listStreets()
        {
            for(auto street : streets)
            {
                cout << street->street_name << endl;
            }
        }
    };

    void static createStreet(vector<string>);
    void static createCity(vector<string>);

    static vector<CityParts::City> cityVector;
    static vector<CityParts::Street> streetVector;
};