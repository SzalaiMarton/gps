#include "new_city.h"
#include "../custom_exception.h"
#include "../display/display.h"

Street::Street(const std::string& name)
{
    this->changeName(name);
    this->connectedStreetsBack = *new std::vector<Street*>();
    this->connectedStreetsFront = *new std::vector<Street*>();
    this->backFull = false;
    this->frontFull = false;
    this->root = false;
}

Street::Street(const std::string& name, std::vector<Street*>& back, std::vector<Street*>& front)
{
    this->changeName(name);
    this->connectedStreetsBack = back;
    this->connectedStreetsFront = front;
    this->backFull = false;
    this->frontFull = false;
    this->root = false;
}

void Street::addStreetToBack(Street *street)
{
    if (street == nullptr)
    {
        throw CustomExceptions::NullInputException("Street::addStreetToBack");
        return;
    }
    
    this->connectedStreetsBack.emplace_back(street);
    if (this->connectedStreetsBack.size() == this->maxBackStreetsSize)
    {
        this->backFull;
    }
}

void Street::addStreetToFront(Street *street)
{
    if (street == nullptr)
    {
        throw CustomExceptions::NullInputException("Street::addStreetToFront");
        return;
    }

    this->connectedStreetsFront.emplace_back(street);
    if (this->connectedStreetsFront.size() == this->maxFrontStreetsSize)
    {
        this->frontFull;
    }
}

void Street::changeTexture(sf::Texture* texture)
{
    if (texture == nullptr)
    {
        throw CustomExceptions::NullInputException("City::changeTexture");
        return;
    }
    this->shape.setTexture(*texture);
}

int Street::getBackStreetsCount()
{
    return this->connectedStreetsBack.size();
}

int Street::getFrontStreetsCount()
{
    return this->connectedStreetsFront.size();
}

std::vector<Street *> Street::getConnectedStreetsBack()
{
    return this->connectedStreetsBack;
}

std::vector<Street *> Street::getConnectedStreetsFront()
{
    return this->connectedStreetsFront;
}

std::string Street::getName()
{
    return this->name;
}



void Street::changeName(const std::string& name)
{
    if (name.length() <= 30)
    {
        this->name = name;
    }
}

City::City(const std::string& name)
{
    this->changeName(name);
    this->streets = *new std::vector<Street*>();
}

City::City(const std::string& name, std::vector<Street*> streets)
{
    this->changeName(name);
    this->streets = streets;
}

int City::getStreetsCount()
{
    return this->streets.size();
}

int City::getUnfinishedCount()
{
    return this->unfinishedStreets.size();
}

std::vector<Street*> City::getStreets()
{
    return this->streets;
}

void City::changeName(const std::string& name)
{
    if (name.length() <= 30)
    {
        this->name = name;
    }
}

void City::changeTexture(sf::Texture* texture)
{
    if (texture == nullptr)
    {
        throw CustomExceptions::NullInputException("City::changeTexture");
        return;
    }
    this->shape.setTexture(*texture);
}

void City::addStreet(Street *street)
{
    if (street == nullptr)
    {
        throw CustomExceptions::NullInputException("City::addStreet");
        return;
    }
    this->streets.emplace_back(street);
}

void City::addUnfinishedStreet(Street *street)
{
    if (street == nullptr)
    {
        throw CustomExceptions::NullInputException("City::addUnfinishedStreet");
        return;
    }
    this->unfinishedStreets.emplace_back(street);
}

void City::removeUnfinishedStreet(Street *street)
{
    if (street == nullptr)
    {
        throw CustomExceptions::NullInputException("City::removeUnfinishedStreet");
        return;
    }
    
    for (int index = 0; index < this->unfinishedStreets.size(); index++)
    {
        if (this->unfinishedStreets[index] == street)
        {
            this->unfinishedStreets.erase(this->unfinishedStreets.begin() + index);
            return;
        }
    }
}

Street *City::getFirstUnfinishedStreet()
{
    return this->unfinishedStreets[0];
}

void City::printStreets(bool printConnected)
{
    for (int index = 0; index < this->streets.size(); index++)
    {
        int displayIndex = index;
        std::cout << displayIndex + 1 << " " << this->streets[index]->getName() << std::endl;
        if (printConnected)
        {
            std::cout << "\tConnected Back: ";
            for (Street* conStr : this->streets[index]->getConnectedStreetsBack())
            {
                std::cout << conStr->getName() << ", ";
            }
            std::cout << "\n\tConnected Front: ";
            for (Street* conStr : this->streets[index]->getConnectedStreetsFront())
            {
                std::cout << conStr->getName() << ", ";
            }
            std::cout << "\n\n";
        }
    }
}

City* CityFunctions::generateCity(const std::string& name, int maxStreetCount)
{
    // generates a city with the given properties
    // char name[] - city's name
    // int streetCount - how many streets will the city has
    // return &resCity - address of the new city

    City* resCity = new City(name);
    std::vector<Street*> unfinishedStreets{};
    std::vector<std::string> randomNames = CityFunctions::getRandomNamesFromFile();
    while(resCity->getStreetsCount() != maxStreetCount)
    {
        if (resCity->getStreetsCount() == 0) // if city is empty then create a root street from where the other streets will expand
        {
            std::string name = CityFunctions::getRandomName(randomNames);
            Street* rootStreet = CityFunctions::createRootStreet(name, resCity);
            resCity->addStreet(rootStreet);
            Display::displayStreet(rootStreet);
            bool canContinue = CityFunctions::connectStreetsToStreet(rootStreet, resCity, randomNames, maxStreetCount);
            if (!canContinue)
            {
                return resCity;
            }
        }
        else // if not empty then grab the frist element from the unfinishedStreets and connect streets to it
        {
            Street* unfinishedStreet = resCity->getFirstUnfinishedStreet();
            bool canContinue = CityFunctions::connectStreetsToStreet(unfinishedStreet, resCity, randomNames, maxStreetCount);
            if (!canContinue)
            {
                return resCity;
            }
        }
    }
    return resCity;
}

std::string CityFunctions::getRandomName(std::vector<std::string>& names)
{
    if (names.size() == 0)
    {
        /*std::cout << "read from file" << std::endl;
        names = CityFunctions::getRandomNamesFromFile();*/
        throw CustomExceptions::VectorEmptyException("CityFunctions::getRandomName");
        return nullptr;
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, names.size() - 1);
    
    int nameIndex = distrib(gen);
    std::string streetName = names[nameIndex];
    names.erase(names.begin() + nameIndex);
    return streetName;
}

std::vector<std::string> CityFunctions::getRandomNamesFromFile()
{
    std::vector<std::string> names;
    const char path[] = "names_new/names.txt";
    std::ifstream file(path);
    std::string line;
    while(std::getline(file, line))
    {
        names.emplace_back(line); 
    }
    file.close();
    return names;
}

Street* CityFunctions::createRootStreet(const std::string& name, City* city)
{
    city->rootStreet = new Street(name);
    city->rootStreet->root = true;
    return city->rootStreet;
}

bool CityFunctions::connectStreetsToStreet(Street *target, City* city, std::vector<std::string> randomNames, int maxStreetCount)
{
    // gets a street and tries to connect more streets to it if the street count hasn't reached the max capacity
    // return   -> returns true if it hasn't reached the max capacity
    //          -> returns false if it has reached the max capacity

    if (!target->backFull)
    {
        for (uint8_t index = 0; index < target->maxBackStreetsSize; index++)
        {
            // removed a connectedStreets.size check because the addStreet func checks if the vector is full or not
            // might have to be added back in the future

            if (city->getStreetsCount()-1 == maxStreetCount)
            {
                return false;
            }

            Street* tobeAddedStreet = new Street(CityFunctions::getRandomName(randomNames));
            target->addStreetToBack(tobeAddedStreet);
            tobeAddedStreet->addStreetToFront(target);
            city->addStreet(tobeAddedStreet);
            city->addUnfinishedStreet(tobeAddedStreet);
        }
        target->backFull = true;
    }
    if (!target->frontFull)
    {
        for (uint8_t index = 0; index < target->maxBackStreetsSize; index++)
        {
            // removed a connectedStreets.size check because the addStreet func checks if the vector is full or not
            // might have to be added back in the future

            if (city->getStreetsCount()-1 == maxStreetCount)
            {
                return false;
            }

            Street* tobeAddedStreet = new Street(CityFunctions::getRandomName(randomNames));
            target->addStreetToFront(tobeAddedStreet);
            tobeAddedStreet->addStreetToBack(target);
            city->addStreet(tobeAddedStreet);
            city->addUnfinishedStreet(tobeAddedStreet);
        }
        target->frontFull = true;
    }
    city->removeUnfinishedStreet(target);
    return true;
}

CityFunctions::VectorSide CityFunctions::getWhereToSnap(Street* target, Street* connectTo)
{
    for (const auto& el : connectTo->getConnectedStreetsBack())
    {
        if (el == target) {
            return CityFunctions::VECTOR_BACK;
        }
    }
    for (const auto& el : connectTo->getConnectedStreetsFront())
    {
        if (el == target) {
            return CityFunctions::VECTOR_FRONT;
        }
    }
    return CityFunctions::VECTOR_INVALID_SIDE;
}
