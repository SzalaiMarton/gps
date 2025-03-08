#include "city.h"

vector<CityParts::City> CityParts::cityVector;
vector<CityParts::Street> CityParts::streetVector;


//FUNCTIONS-------------------------------------------------------
void CityParts::createStreet(vector<string> properties)
{
    CityParts::Street* temp = new CityParts::Street(properties); // [0]-name, [1]-length
    streetVector.push_back(*temp);
    delete temp;
}

void CityParts::createCity(vector<string> properties)
{
    CityParts::City* temp = new CityParts::City(properties); // [0]-name, [1]-density in number
    cityVector.push_back(*temp);
    delete temp;
}

bool CityParts::checkForExistingObject(string name, string obj)
{
    if (obj == "street")
    {
        for (auto el : streetVector)
        {
            if (el.streetName == name)
            {
                return true;
            }
        }
    }
    else if (obj == "city")
    {
        for (auto el : cityVector)
        {
            if (el.cityName == name)
            {
                return true;
            }
        }
    }
    return false;
}

//STREET FUNCTIONS-----------------------------------------------
CityParts::Street::Street(vector<string> properties)
{
    for (int index = 0; index < properties.size(); index++)
    {
        switch (index)
        {
        case 1:
            streetName = properties[index];
            break;
        case 2:
            length = stoi(properties[index]);
            break;
        }
    }
}

void CityParts::Street::attachToStreet(Street* rootStreet, string side)
{
    if(side == "back")
    {
        rootStreet->connectedStreetsBack.push_back(this);
    }
    else if(side == "front")
    {
        rootStreet->connectedStreetsFront.push_back(this);
    }
}


//CITY FUNCTIONS-----------------------------------------------
CityParts::City::City(vector<string> properties)
{
    for (int index = 0; index < properties.size(); index++)
    {
        switch (index)
        {
        case 1:
            if (CityParts::checkForExistingObject(properties[index], "city"))
            {
                cout << "City already exist." << endl;
                return;
            }
            cityName = properties[index];
            break;
        case 2:
            populationInNumber = stoi(properties[index]);
            break;
        }
    }
}

void CityParts::City::printPopulation()
{
    cout << cityName + "'s population: " + to_string(streets.size());
}

void CityParts::City::convertPopNumberToDensity()
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

void CityParts::City::listStreets()
{
    for(auto street : streets)
    {
        cout << street->streetName << endl;
    }
}