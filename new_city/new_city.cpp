#include "new_city.h"

Street::Street(char name[])
{
    this->changeName(name);
    this->connectedStreetsBack = *new std::vector<Street*>();
    this->connectedStreetsFront = *new std::vector<Street*>();
}

Street::Street(char name[], std::vector<Street*>& back, std::vector<Street*>& front)
{
    this->changeName(name);
    this->connectedStreetsBack = back;
    this->connectedStreetsFront = front;
}

void Street::addStreetToBack(Street *street)
{
    this->connectedStreetsBack.emplace_back(street);
}

void Street::addStreetToFront(Street *street)
{
    this->connectedStreetsFront.emplace_back(street);
}

int Street::getBackStreetsCount()
{
    return this->connectedStreetsBack.size();
}

int Street::getFrontStreetsCount()
{
    return this->connectedStreetsFront.size();
}

void Street::changeName(char name[])
{
    if (strlen(name) <= 20)
    {
        strcpy(this->name, name);
    }
}

City::City(char name[])
{
    this->changeName(name);
    this->streets = *new std::vector<Street*>();
}

City::City(char name[], std::vector<Street*> streets)
{
    this->changeName(name);
    this->streets = streets;
}

int City::getStreetsCount()
{
    return this->streets.size();
}

void City::changeName(char name[])
{
    if (strlen(name) <= 20)
    {
        strcpy(this->name, name);
    }
}

void City::addStreet(Street *street)
{
    this->streets.emplace_back(street);
}

void City::addUnfinishedStreet(Street *street)
{
    this->unfinishedStreets.emplace_back(street);
}

Street *City::getFirstUnfinishedStreet()
{
    return this->unfinishedStreets[0];
}

City* CityFunctions::generateCity(char name[], int streetCount, City* city)
{
    City res = *new City(name);
    std::vector<Street*> unfinishedStreets{};
    std::vector<char[]> randomNames = CityFunctions::getRandomNamesFromFile();
    for (int i = 0; i < streetCount; i++)
    {
        if (city->getStreetsCount() == 0)
        {
            char* name = CityFunctions::getRandomName(randomNames);

            Street* rootStreet = CityFunctions::createRootStreet(name);
            CityFunctions::connectStreetsToStreet(rootStreet, city, randomNames);
        }
        else
        {
            Street* unfinishedStreet = city->getFirstUnfinishedStreet();
            CityFunctions::connectStreetsToStreet(unfinishedStreet, city, randomNames);
        }
    }
    return &res;
}

char *CityFunctions::getRandomName(std::vector<char[]>& names)
{
    if (names.size() == 0)
    {
        std::cout << "read from file" << std::endl;
        names = CityFunctions::getRandomNamesFromFile();
    }
    uint16_t nameIndex = rand() % names.size();
    char* streetName = names[nameIndex];
    names.erase(names.begin() + nameIndex);
    return streetName;
}

std::vector<char[]> CityFunctions::getRandomNamesFromFile()
{
    std::vector<char[]> names;
    const char path[] = "names/names.txt";
    std::ifstream file(path);
    std::string line;
    while(std::getline(file, line))
    {
        names.emplace_back(line); 
    }
    file.close();
    return names;
}

Street* CityFunctions::createRootStreet(char name[])
{
    return new Street(name);
}

Street *CityFunctions::connectStreetsToStreet(Street *target, City* city, std::vector<char[]> randomNames)
{
    if (!target->backFull)
    {
        for (uint8_t index = 0; index < target->maxBackStreetsSize; index++)
        {
            if (target->getBackStreetsCount() != target->maxBackStreetsSize)
            {
                Street* tobeAddedStreet = new Street(CityFunctions::getRandomName(randomNames));
                target->addStreetToBack(tobeAddedStreet);
                city->addStreet(tobeAddedStreet);
                city->addUnfinishedStreet(tobeAddedStreet);
            }
        }
        target->backFull = true;
    }
    if (!target->frontFull)
    {
        for (uint8_t index = 0; index < target->maxBackStreetsSize; index++)
        {
            if (target->getFrontStreetsCount() != target->maxFrontStreetsSize)
            {
                Street* tobeAddedStreet = new Street(CityFunctions::getRandomName(randomNames));
                target->addStreetToFront(tobeAddedStreet);
                city->addStreet(tobeAddedStreet);
                city->addUnfinishedStreet(tobeAddedStreet);
            }
        }
        target->frontFull = true;
    }
}
