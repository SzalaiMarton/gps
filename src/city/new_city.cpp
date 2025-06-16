#include "new_city.h"
#include <iostream>
#include <cstdint>
#include <random>
#include <fstream>
#include <sstream>

Street::Street(const std::string& name)
{
    this->name = name;
    this->connectedStreetsBack = std::vector<Street*>();
    this->connectedStreetsFront = std::vector<Street*>();
    this->weight = 0;
    this->isRoot = false;
    this->isFinished = false;
    this->backPoint = nullptr;
    this->frontPoint = nullptr;

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 10 - 1);

    this->weight = distrib(gen);
}

Street::~Street()
{
    if (this->backPoint != nullptr)
        this->backPoint->removeConnection(this);
    if(this->frontPoint != nullptr)
            this->frontPoint->removeConnection(this);
}

bool Street::addStreetToBack(Street *street, bool addToTheOther)
{
    bool canAdd = true;
    
    if (street == nullptr)
        return false;
    
    if (addToTheOther)
        canAdd = street->addStreetToFront(this, false);

    if (this->connectedStreetsBack.size() < this->idealConnectionSize && !this->isStreetConnected(street) && canAdd)
    {
        this->connectedStreetsBack.emplace_back(street);

        if (this->backPoint != nullptr && addToTheOther)
        {
            street->frontPoint = this->backPoint;
            street->frontPoint->connectedTo.emplace_back(street);
            std::cout << street->frontPoint << " front point added to " << street->name << std::endl;
        }

        return true;
    }
    else
        this->isFinished = true;
    return false;
}

bool Street::addStreetToFront(Street *street, bool addToTheOther)
{
    bool canAdd = true;

    if (street == nullptr)
        return false;

    if (addToTheOther)
        canAdd = street->addStreetToBack(this, false);

    if (this->connectedStreetsFront.size() < this->idealConnectionSize && !this->isStreetConnected(street) && canAdd)
    {
        this->connectedStreetsFront.emplace_back(street);
        
        if (this->frontPoint != nullptr && addToTheOther)
        {
            street->backPoint = this->frontPoint;
            street->backPoint->connectedTo.emplace_back(street);
            std::cout << street->backPoint << " back point added to " << street->name << std::endl;
        }

        return true;
    }
    else
        this->isFinished = true;
    return false;
}

bool ConnectionPoint::isStreetConnected(Street* street)
{
    for (auto& str : this->connectedTo)
    {
        if (str == street)
            return true;
    }
    return false;
}


bool Street::isStreetConnected(Street* street)
{
    for (const auto& str : this->connectedStreetsBack)
    {
        if (str == street)
            return true;
    }
    for (const auto& str : this->connectedStreetsFront)
    {
        if (str == street)
            return true;
    }
    return false;
}

CityFunctions::VectorSide Street::getEmptyVectorSide() const
{
    if (this->connectedStreetsBack.size() < this->idealConnectionSize)
        return CityFunctions::VECTOR_BACK;
    else if (this->connectedStreetsFront.size() < this->idealConnectionSize)
        return CityFunctions::VECTOR_FRONT;
    else
        return CityFunctions::VECTOR_INVALID_SIDE;
}

City::City(const std::string& name)
{
    this->name = name;
    this->streets = std::vector<Street*>();
    this->points = std::vector<ConnectionPoint*>();
    this->rootStreet = nullptr;
}

City::~City()
{
    for (int i = 0; i < this->streets.size(); i++)
    {
        delete this->streets[i];
    }
    this->streets.clear();
    rootStreet = nullptr;

    for (int i = 0; i < this->points.size(); i++)
    {
        delete this->points[i];
    }
    this->points.clear();
}

int City::getUnfinishedCount() const
{
    return (int)this->unfinishedStreets.size();
}

void City::addStreet(Street *street)
{
    if (street == nullptr)
    {
        return;
    }
    this->streets.emplace_back(street);
}

void City::addUnfinishedStreet(Street *street)
{
    if (street == nullptr)
    {
        return;
    }
    this->unfinishedStreets.emplace_back(street);
}

void City::removeFinishedFromUnfinished()
{
    std::vector<std::vector<Street*>::iterator> strIndexesToBeDeleted{};

    auto it = this->unfinishedStreets.begin();

    while (it != this->unfinishedStreets.end())
    {
        if ((*it)->isFinished)
            it = this->unfinishedStreets.erase(it);
        else
            ++it;
    }
}

Street* City::getFirstUnfinishedStreet()
{
    for (const auto& str : this->unfinishedStreets)
    {
        if (!str->isFinished)
            return str;
    }
    return nullptr;
}

Street* City::getMiddleUnfinishedStreet()
{
    if (!this->unfinishedStreets.empty() && this->unfinishedStreets.size() > 1)
    {
        return this->unfinishedStreets[(int)this->unfinishedStreets.size() / 2];
    }
    else if (this->unfinishedStreets.size() == 1)
    {
        return this->getFirstUnfinishedStreet();
    }
    return nullptr;
}

void City::printStreets(bool printConnected)
{
    for (int index = 0; index < this->streets.size(); index++)
    {
        int displayIndex = index;
        std::cout << displayIndex + 1 << " " << this->streets[index]->name << " weight: " << (int)this->streets[index]->weight << std::endl;
        std::cout << "backPoint: " << this->streets[index]->backPoint << " frontPoint: " << this->streets[index]->frontPoint << std::endl;
        if (printConnected)
        {
            std::cout << "\tConnected Back: ";
            for (Street* conStr : this->streets[index]->connectedStreetsBack)
            {
                std::cout << conStr->name << ", ";
            }
            std::cout << "\n\tConnected Front: ";
            for (Street* conStr : this->streets[index]->connectedStreetsFront)
            {
                std::cout << conStr->name << ", ";
            }
            std::cout << "\n\n";
        }
    }
}

void City::printUnfinished()
{
    for (const auto& str : this->unfinishedStreets)
    {
        std::cout << "unfinished name: " << str->name << std::endl;
    }
}

City* CityFunctions::generateCity(const std::string& name, int maxStreetCount)
{
    // just generate maxStreetCount amount of streets
    // then connect them to eachothers
    // then display them

    City* resCity = new City(name);
    std::vector<std::string> randomNames = CityFunctions::getRandomNamesFromFile();

    resCity->rootStreet = CityFunctions::createRootStreet(CityFunctions::getRandomName(randomNames), resCity);
    resCity->addStreet(resCity->rootStreet);
    resCity->addUnfinishedStreet(resCity->rootStreet);

    while (resCity->streets.size() != maxStreetCount)
    {
        Street* street = new Street(CityFunctions::getRandomName(randomNames));
        resCity->addStreet(street);
        resCity->addUnfinishedStreet(street);
    }

    while (!(resCity->unfinishedStreets.size() <= 5))
    {
        resCity->connectStreets();
        std::cout << "***********************************************\n";
    }

    //resCity->printAllStreetReferenceCount();
    resCity->printStreets(true);

    //leave it for later
    //Display::displayCity(resCity);

    return resCity;
}

std::string CityFunctions::vectorSideToString(VectorSide side)
{
    if (VECTOR_FRONT == side)
        return "vector_front";
    else if (VECTOR_BACK == side)
        return "vector_back";
    else if (VECTOR_INVALID_SIDE == side)
        return "vector_invalid";
}

std::string CityFunctions::getRandomName(std::vector<std::string>& names)
{
    if (names.empty())
    {
        throw std::runtime_error("No names available to select.");
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
    city->rootStreet->isRoot = true;
    city->rootStreet->backPoint = new ConnectionPoint(city->rootStreet);
    city->rootStreet->frontPoint = new ConnectionPoint(city->rootStreet);

    std::cout << "back point for root street: " << city->rootStreet->backPoint << std::endl;
    std::cout << "front point for root street: " << city->rootStreet->frontPoint << std::endl;

    return city->rootStreet;
}

void City::connectStreets()
{
    CityFunctions::VectorSide vectorSide{};
    Street* connectTo = this->getFirstUnfinishedStreet();
    
    std::cout << "current connectTo: " << connectTo->name << std::endl;

    for (int index = 0; index < this->unfinishedStreets.size(); index++)
    {
        if (this->unfinishedStreets[index] == connectTo)
            continue;

        vectorSide = connectTo->getEmptyVectorSide();

        if (vectorSide == CityFunctions::VECTOR_BACK && this->unfinishedStreets[index]->frontPoint == nullptr)
        {
            std::cout << "adding to back " << this->unfinishedStreets[index]->name << std::endl;
            connectTo->addStreetToBack(this->unfinishedStreets[index], true);
        }
        else if (vectorSide == CityFunctions::VECTOR_FRONT && this->unfinishedStreets[index]->backPoint == nullptr)
        {
            std::cout << "adding to front " << this->unfinishedStreets[index]->name << std::endl;
            connectTo->addStreetToFront(this->unfinishedStreets[index], true);
        }
        else
        {
            connectTo->isFinished = true;
            connectTo = this->getFirstUnfinishedStreet();
            std::cout << "current connectTo: " << connectTo->name << std::endl;
            connectTo->createPointToStreet();
            if (connectTo == nullptr)
            {
                std::cout << "nullptr at City::connectStreets()" << std::endl;
                return;
            }
            index--;
        }
        std::cout << "----------------------------------" << std::endl;
    }

    this->removeFinishedFromUnfinished();
}

void City::connectToRandomUnfinishedStreet(Street* street)
{
    Street* randomUnfinished = this->getMiddleUnfinishedStreet();

    randomUnfinished->addStreetToBack(street, true);
}

void City::getShortestRoute(const std::string& from, const std::string& to, City* city)
{
    
}

void City::printAllStreetReferenceCount()
{
    int currentStrReferenceBack = 0;
    int currentStrReferenceFront = 0;

    for (const auto& str : this->streets)
    {
        for (const auto& innerStr : this->streets)
        {
            if (str == innerStr)
                continue;

            for (const auto& backStr : innerStr->connectedStreetsBack)
            {
                if (str == backStr)
                    currentStrReferenceBack++;
            }
            for (const auto& frontStr : innerStr->connectedStreetsFront)
            {
                if (str == frontStr)
                    currentStrReferenceFront++;
            }
        }

        if (currentStrReferenceBack > 3)
            std::cout << str->name << " was referenced at back: " << currentStrReferenceBack << " times" << std::endl;
        if (currentStrReferenceFront > 3)
            std::cout << str->name << " was referenced at back: " << currentStrReferenceFront << " times" << std::endl;

        currentStrReferenceBack = 0;
        currentStrReferenceFront = 0;
    }
}

ConnectionPoint::ConnectionPoint(Street* street)
{
    this->connectedTo = std::vector<Street*>();
    this->connectedTo.emplace_back(street);
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 10 - 1);

    this->weight = distrib(gen);
}

void ConnectionPoint::removeConnection(Street* street)
{
    if (this->isStreetConnected(street))
    {
        for (int i = 0; i < this->connectedTo.size(); i++)
        {
            if (this->connectedTo[i] == street)
            {
                this->connectedTo.erase(this->connectedTo.begin() + i);

                if (street->backPoint == this)
                    street->backPoint = nullptr;
                else 
                    street->frontPoint = nullptr;

                return;
            }
        }
    }
}

void Street::createPointToStreet()
{
    if (this->backPoint == nullptr)
    {
        this->backPoint = new ConnectionPoint(this);
        std::cout << this->backPoint << "created back for " << this->name << std::endl;
    }
    if (this->frontPoint == nullptr)
    {
        this->frontPoint = new ConnectionPoint(this);
        std::cout << this->frontPoint << " created front for " << this->name << std::endl;
    }
}