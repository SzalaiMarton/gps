#pragma once

#include <string>
#include <vector>

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
        vector<Buildings>* buildings;
        string street_name;
        vector<Street*> connected_streets_front;
        vector<Street*> connected_streets_back;
        Street(string name, int street_length)
        {
            street_name = name;
        }
    };

    struct City
    {
        string cityName;
        City(Density population)
        {
            vector<Street*> collection_of_streets;
            collection_of_streets.resize(population);
        }
    };

    void createStreet(vector<string>);

    static vector<CityParts::City> cityVector;
    static vector<CityParts::Street> streetVector;
};