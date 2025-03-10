#include "city.h"
#include "commands.h"
#include <fstream>

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

CityParts::Street* CityParts::generateRandomStreet(string cityName, vector<string> &randomNames)
{
    int nameIndex = rand() % randomNames.size();
    string streetName = randomNames[nameIndex];
    randomNames.erase(randomNames.begin() + nameIndex);
    CityParts::Street* temp = new CityParts::Street({streetName});
    temp->city = cityName;
    streetVector.push_back(*temp);
    cout << temp->streetName << " has been created." << endl;
    Street* returnTemp = temp;
    delete temp;
    return returnTemp;
}

vector<string> CityParts::randomNameGenerator()
{
    string path = "names//names.txt";
    ifstream file(path);
    vector<string> names;
    string line;
    while(getline(file, line))
    {
        names.push_back(line); 
    }
    file.close();
    return names;
}

void CityParts::generateRandomCity(string cityName, int numberOfStreets, int maxConnectedStreet)
{
    //recommended number of streets is 50;
    vector<string> randomNames = CityParts::randomNameGenerator();
    CityParts::City* temp = new CityParts::City({cityName});
    cityVector.push_back(*temp);
    cout << temp->cityName << " city has been created." << endl;
    delete temp;
    if (maxConnectedStreet < 2)
    {
        cout << "Invalid number of connected streets." << endl;
        return;
    }
    int cityIndex = getCityIndex(cityName);

    //create a street then attach maxConnectedStreet - 1 to it and then attach each streets to eachothers connected back
    //so it is always creates a crossroad
    vector<Street*> unfinishedStreets;
    for (int index = 0; index < numberOfStreets; index++)
    {
        unfinishedStreets.push_back(CityParts::generateRandomStreet(cityName, randomNames));
        for (auto street : unfinishedStreets)
        {
            if (street->connectedStreetsBack.size() < maxConnectedStreet)
            {
                //creating streets
                Street* tempStreet1 = CityParts::generateRandomStreet(cityName, randomNames);
                Street* tempStreet2 = CityParts::generateRandomStreet(cityName, randomNames);
                Street* tempStreet3 = CityParts::generateRandomStreet(cityName, randomNames);
                //connecting streets to eachother
                street->attachStreet(tempStreet1, "back");
                street->attachStreet(tempStreet2, "back");
                street->attachStreet(tempStreet3, "back");
                
                tempStreet1->attachStreet(street, "front");
                tempStreet1->attachStreet(tempStreet2, "front");
                tempStreet1->attachStreet(tempStreet3, "front");

                tempStreet2->attachStreet(street, "front");
                tempStreet2->attachStreet(tempStreet1, "front");
                tempStreet2->attachStreet(tempStreet3, "front");

                tempStreet3->attachStreet(street, "front");
                tempStreet3->attachStreet(tempStreet1, "front");
                tempStreet3->attachStreet(tempStreet2, "front");
                //pushing streets to the vector
                streetVector.push_back(*tempStreet1);
                streetVector.push_back(*tempStreet2);
                streetVector.push_back(*tempStreet3);
                //deleting temp streets
                delete tempStreet1;
                delete tempStreet2;
                delete tempStreet3;
            }
        }
    }
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

int CityParts::getCityIndex(string name)
{
    for (int index = 0; index < cityVector.size(); index++)
    {
        if (cityVector[index].cityName == name)
        {
            return index;
        }
    }
    return -1;
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
        }
    }
}

void CityParts::Street::attachStreet(Street* streetToBeAttached, string side)
{
    if(side == "back")
    {
        this->connectedStreetsBack.push_back(streetToBeAttached);
    }
    else if(side == "front")
    {
        this->connectedStreetsFront.push_back(streetToBeAttached);
    }
}

void CityParts::Street::listProperties()
{
    cout << "[Properties] " << "name(name): " << this->streetName << endl;
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
        }
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


void CityParts::City::editStreets(string action)
{
    Commands command;
    //take input then to add street or remove
    string input;
    if (action == command.EDIT_APPEND_COMMAND)
    {
        do
        {
            cout << "To exit editing panel type: exit " << endl;
            cout << "Type in the name of the street you want to attach: ";
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (CityParts::checkForExistingObject(input, "street"))
            {
                for (auto street : CityParts::streetVector)
                {
                    if (street.streetName == input)
                    {
                        this->streets.push_back(&street);
                        cout << "Street has been attached." << endl;
                        return;
                    }
                }
            }
            else
            {
                cout << "Street doesn't exist." << endl;
            }
        } while (input != "exit");
    }
    else
    {
        do
        {
            cout << "To exit editing panel type: exit " << endl;
            cout << "Type in the name of the street you want to detach: ";
            cin >> input;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            for (int index = 0; index < this->streets.size(); index++)
            {
                if (this->streets[index]->streetName == input)
                {
                    this->streets.erase(this->streets.begin() + index);
                    cout << "Street has been detached." << endl;
                    return;
                }
            }
            cout << "Street not found." << endl;
        } while (input != "exit");
    }
}