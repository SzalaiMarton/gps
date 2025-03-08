#include "city.h"

vector<CityParts::City> CityParts::cityVector;
vector<CityParts::Street> CityParts::streetVector;


//FUNCTIONS-------------------------------------------------------
void CityParts::createStreet(vector<string> properties)
{
    CityParts::Street* temp = new CityParts::Street(properties); // [0]-name, [1]-length
    streetVector.push_back(*temp);
    cout << temp->streetName << " has been created." << endl;
    delete temp;
}

void CityParts::createCity(vector<string> properties)
{
    CityParts::City* temp = new CityParts::City(properties); // [0]-name, [1]-density in number
    cityVector.push_back(*temp);
    cout << temp->cityName << " has been created." << endl;
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

CityParts::Street::Street()
{
    this->streetName = "default";
    this->length = 0;
    this->city = "None";
}

// STREET FUNCTIONS-----------------------------------------------
CityParts::Street::Street(vector<string> properties)
{
    CityParts::Street();
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

void CityParts::Street::listProperties()
{
    cout << "[Properties] " << "name: " << this->streetName << endl;
    cout << "[Properties] " << "length: " << this->length << endl;
    cout << "[Properties] " << "located in: " << this->city << endl;
    cout << "[Properties] " << "buildings: " << endl;
    this->listBuildings();
    cout << "[Properties] " << "buildings-----------------------------" << endl;
    cout << "[Properties] " << "connected streets back: " << endl;
    this->listConnectedStreetsBack();
    cout << "[Properties] " << "connected streets back----------------" << endl;
    cout << "[Properties] " << "connected streets front: " << endl;
    this->listConnectedStreetsFront();
    cout << "[Properties] " << "connected streets front---------------" << endl;
}

void CityParts::Street::listBuildings()
{
    if (this->buildings.empty() || this->buildings.size() == 0)
    {
        cout << "No buildings have been attached." << endl;
        return;
    }
    int index = 0;
    for (CityParts::Buildings el : this->buildings)
    {
        index++;
        cout << "   " << index << el.buildingName << endl;
    }
}

void CityParts::Street::listConnectedStreetsBack()
{
    if (this->connectedStreetsBack.empty() || this->connectedStreetsBack.size() == 0)
    {
        cout << "No streets have been attached to the back." << endl;
        return;
    }
    int index = 0;
    for (auto el : this->connectedStreetsBack)
    {
        index++;
        cout << "   " << index << el->streetName << endl;
    }
}

void CityParts::Street::listConnectedStreetsFront()
{
    if (this->connectedStreetsFront.empty() || this->connectedStreetsFront.size() == 0)
    {
        cout << "No streets have been attached to the front." << endl;
        return;
    }
    int index = 0;
    for (auto el : this->connectedStreetsFront)
    {
        index++;
        cout << "   " << index << el->streetName << endl;
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
    if (this->streets.empty() || this->streets.size() == 0)
    {
        cout << "No street have been attached." << endl;
        return;
    }
    int index = 0;
    for(auto street : streets)
    {
        index++;
        cout << "   " << index << street->streetName << endl;
    }
}

void CityParts::City::listProperties()
{
    cout << "[Properties] " << "name: " << this->cityName << endl;
    cout << "[Properties] " << "population: " << this->populationInNumber << " " << CityParts::DensityUtils::toString(this->populationInDensity) << endl;
    cout << "[Properties] " << "attached streets: " << endl;
    CityParts::City::listStreets();
    cout << "[Properties] " << "attached streets----------------------" << endl;
}

void CityParts::City::editName(string name)
{
    this->cityName = name;
}

void CityParts::City::editPopulation(int number)
{
    this->populationInNumber = number;
}

void CityParts::City::editStreets()
{
    //list available streets
    //take input then to add street or remove
}