#include "city.h"
#include "commands.h"

vector<CityParts::City> CityParts::cityVector;
vector<CityParts::Street> CityParts::streetVector;

//FUNCTIONS-------------------------------------------------------
void CityParts::createStreet(vector<string> properties)
{
    if (properties[1].size() == 2)
    {
        cout << "Invalid name, it should be 3 characters long" << endl;
        return;
    }
    CityParts::Street* temp = new CityParts::Street(properties); // [1]-name, [2]-length
    streetVector.push_back(*temp);
    cout << temp->streetName << " has been created." << endl;
    delete temp;
}

void CityParts::createCity(vector<string> properties)
{
    if (properties[1].size() == 2)
    {
        cout << "Invalid name, it should be 3 characters long" << endl;
        return;
    }
    if (CityParts::checkForExistingObject(properties[1], "city"))
    {
        cout << "City already exist." << endl;
        return;
    }
    CityParts::City* temp = new CityParts::City(properties); // [1]-name, [2]-density in number
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

bool CityParts::checkIfStreetInCityAlready(string targetName, string cityName)
{
    for (auto street : CityParts::streetVector)
    {
        if (street.streetName == targetName && street.city == cityName)
        {
            return true;
        }
    }
    return false;
}

// STREET FUNCTIONS-----------------------------------------------
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

void CityParts::Street::listProperties()
{
    cout << "[Properties] " << "name(name): " << this->streetName << endl;
    cout << "[Properties] " << "length(length): " << this->length << endl;
    cout << "[Properties] " << "located in(city): " << this->city << endl;
    cout << "[Properties] " << "buildings(buildings): " << endl;
    this->listBuildings();
    cout << "[Properties] " << "buildings-----------------------------" << endl;
    cout << "[Properties] " << "connected streets back(sback): " << endl;
    this->listConnectedStreetsBack();
    cout << "[Properties] " << "connected streets back----------------" << endl;
    cout << "[Properties] " << "connected streets front(sfront): " << endl;
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

void CityParts::Street::editName()
{
    string name;
    do
    {
        cout << "To quit editing type: exit" << endl;
        cout << "New value: ";
        cin >> name;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!CityParts::checkIfStreetInCityAlready(name, this->city))
        {
            this->streetName = name;
            cout << "Name has been changed" << endl;
            return;
        }
        else
        {
            cout << "Street already exist in this city" << endl;
        }
    } while(name != "exit");
}

void CityParts::Street::editLength()
{
    string number;
    do
    {
        cout << "To quit editing type: exit" << endl;
        cout << "New value: ";
        cin >> number;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (stoi(number) >= 0)
        {
            this->length = stoi(number);
            cout << "Length has been changed" << endl;
            return;
        }
        else
        {
            cout << "Invalid input, it has to be a number which is positive" << endl;;
        }
    } while(number != "exit");
}

void CityParts::Street::editCity()
{
    string name;
    do
    {
        cout << "To quit editing type: exit" << endl;
        cout << "New value: ";
        cin >> name;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (CityParts::checkForExistingObject(name, "city"))
        {
            this->city = name;
            cout << "City has been changed" << endl;
            return;
        }
        else
        {
            cout << "City doesn't exist" << endl;
        }
    } while(name != "exit");
}

void CityParts::Street::editBuildings(string action, CityParts::Buildings target)
{
    Commands command;
    if (action == command.EDIT_DELETE_COMMAND)
    {
        for (int index = 0; index < this->buildings.size(); index++)
        {
            if (this->buildings[index].buildingName == target.buildingName)
            {
                this->buildings.erase(this->buildings.begin() + index);
                cout << "Building successfully deleted." << endl;
                return;
            }
        }
    }
    else if (action == command.EDIT_APPEND_COMMAND)
    {
        this->buildings.push_back(target);
    }
}

void CityParts::Street::editConnectedStreetsBack(string action, CityParts::Street* target)
{
    Commands command;
    if (action == command.EDIT_DELETE_COMMAND)
    {
        for (int index = 0; index < this->connectedStreetsBack.size(); index++)
        {
            if (this->connectedStreetsBack[index]->streetName == target->streetName)
            {
                this->connectedStreetsBack.erase(this->connectedStreetsBack.begin() + index);
                cout << "Street successfully deleted." << endl;
                return;
            }
        }
    }
    else if (action == command.EDIT_APPEND_COMMAND)
    {
        this->connectedStreetsBack.push_back(target);
    }
}

void CityParts::Street::editConnectedStreetsFront(string action, CityParts::Street* target)
{
    Commands command;
    if (action == command.EDIT_DELETE_COMMAND)
    {
        for (int index = 0; index < this->connectedStreetsFront.size(); index++)
        {
            if (this->connectedStreetsFront[index]->streetName == target->streetName)
            {
                this->connectedStreetsFront.erase(this->connectedStreetsFront.begin() + index);
                cout << "Street successfully deleted." << endl;
                return;
            }
        }
    }
    else if (action == command.EDIT_APPEND_COMMAND)
    {
        this->connectedStreetsFront.push_back(target);
    }
}

void CityParts::Street::deleteCity()
{
    this->city = "None";
}

// CITY FUNCTIONS-----------------------------------------------
CityParts::City::City(vector<string> properties)
{
    for (int index = 0; index < properties.size(); index++)
    {
        switch (index)
        {
        case 1:
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
    cout << "[Properties] " << "name(name): " << this->cityName << endl;
    cout << "[Properties] " << "population(population): " << this->populationInNumber << " " << CityParts::DensityUtils::toString(this->populationInDensity) << endl;
    cout << "[Properties] " << "attached streets(streets): " << endl;
    CityParts::City::listStreets();
    cout << "[Properties] " << "attached streets----------------------" << endl;
}

void CityParts::City::editName()
{
    string name;
    do
    {
        cout << "To quit editing type: exit" << endl;;
        cout << "New value: ";
        cin >> name;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (!CityParts::checkForExistingObject(name, "city"))
        {
            this->cityName = name;
            cout << "Name has been changed" << endl;
            return;
        }
        else
        {
            cout << "City already exist" << endl;
        }
    } while(name != "exit");
}

void CityParts::City::editPopulation()
{
    string number;
    do
    {
        cout << "To quit editing type: exit" << endl;;
        cout << "New value: ";
        cin >> number;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (stoi(number) >= 0)
        {
            this->populationInNumber = stoi(number);
            cout << "Population has been changed" << endl;
            this->convertPopNumberToDensity();
            return;
        }
        else
        {
            cout << "Invalid input, it has to be a number which is positive" << endl;;
        }
    } while(number != "exit");
}

void CityParts::City::editStreets(string action)
{
    //take input then to add street or remove
}