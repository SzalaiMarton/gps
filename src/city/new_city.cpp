#include "new_city.h"
#include <iostream>
#include <cstdint>
#include <random>
#include <fstream>
#include <sstream>

static std::random_device rd;
static std::mt19937 gen(rd());

namespace CityFunctions
{
    std::vector<std::string> pointNames = CityFunctions::readFile("names/point_names.txt");
    std::vector<std::string> streetNames = CityFunctions::readFile("names/street_names.txt");
    std::vector<ConnectionPoint*> candidatePoints{};
    std::vector<sf::Sprite*> route{};
}

Street::Street()
{
    this->name = CityFunctions::getRandomName(&CityFunctions::streetNames);
    this->backPoint = nullptr;
    this->frontPoint = nullptr;
    this->goFront = false;
    this->goBack = false;
    this->shape = new sf::Sprite();

    std::uniform_int_distribution<> distrib(1, 9);
    this->weight = distrib(gen);

    this->appeared = 0;
}

Street::~Street()
{
    if (this->backPoint != nullptr)
        this->backPoint->removeStreet(this);
    if(this->frontPoint != nullptr)
        this->frontPoint->removeStreet(this);
}

bool ConnectionPoint::addPoint(ConnectionPoint* point, bool& isAlreadyIn)
{
    if (point == this)
    {
        DEBUG_LOG("[IDENTICAL POINT] cannot add point to itself. " << point->name);
        return false;
    }
    else if (this->isFull())
    {
        DEBUG_LOG("[POINT FULL] cannot add more streets to " << this->name);
        CityFunctions::removeFromCandidates(this);
        return false;
    }
    else if (this->isPointAlreadyConnected(point))
    {
        DEBUG_LOG("[ALREADY CONNECTED POINT] " << point->name << " is already connected to " << this->name);
        isAlreadyIn = true;
        return true;
    }
    else
    {
        DEBUG_LOG("[ADDITION] " << point->name << " was added to " << this->name);
        this->connectedPoints.emplace_back(point);

        if (this->isFull())
            CityFunctions::removeFromCandidates(this);
        
        return true;
    }
}

void ConnectionPoint::removePoint(ConnectionPoint* point)
{
    auto index = std::find(this->connectedPoints.begin(), this->connectedPoints.end(), point);
    if (index == this->connectedPoints.end())
    {
        DEBUG_LOG("[REMOVE POINT NOT FOUND] " << point->name << " was not found in " << this->name << "'s connections");
    }
    else
    {
        DEBUG_LOG("[REMOVAL] " << point->name << " was removed from " << this->name);
        this->connectedPoints.erase(index);
    }
}

void ConnectionPoint::removeStreet(Street* street)
{
    auto index = std::find(this->connectedStreets.begin(), this->connectedStreets.end(), street);
    if (index == this->connectedStreets.end())
    {
        DEBUG_LOG("[REMOVE STREET NOT FOUND] " << street->name << " was not found in " << this->name << "'s connections");
    }
    else
    {
        this->connectedStreets.erase(index);
        DEBUG_LOG("[REMOVE STREET] " << street->name << " has been removed from " << this->name << "'s connections");
    }
}

bool ConnectionPoint::isStreetConnected(Street* street)
{
    for (auto& str : this->connectedStreets)
    {
        if (str == street)
            return true;
    }
    return false;
}

City::City(const std::string& name)
{
    this->name = name;
    this->firstDisplayPoint = nullptr;
    this->streets = std::vector<Street*>();
    this->points = std::vector<ConnectionPoint*>();
}

City::~City()
{
    for (int i = 0; i < this->streets.size(); i++)
    {
        delete this->streets[i];
    }
    this->streets.clear();

    for (int i = 0; i < this->points.size(); i++)
    {
        delete this->points[i];
    }
    this->points.clear();
}

void City::addStreet(Street *street)
{
    if (street != nullptr)
        this->streets.emplace_back(street);
}

void City::deleteStreet(Street* street)
{
    auto it = std::find(this->streets.begin(), this->streets.end(), street);
    if (it != this->streets.end())
    {
        this->streets.erase(it);
        DEBUG_LOG("[DELETION] " << street->name << " has been deleted.");
        delete street;
    }
    else
        DEBUG_LOG("[DELETION ERROR] failed to delete " << street->name << "'s connections");
}

void City::addPoint(ConnectionPoint* point, bool isDisplayed)
{
    if (point != nullptr)
        this->points.emplace_back(point);
    if (!isDisplayed)
        this->toBeDisplayedPoints.emplace_back(point);
}

City* CityFunctions::generateCity(const std::string& name, int pointCount)
{
    City* createdCity = new City(name);

    LOG("Creating points for new city.");
    for (int i = 0; i < pointCount; i++)
    {
        createdCity->addPoint(new ConnectionPoint(), false);
    }
    LOG("Finished creating points for new city.");
    
    LOG("Connecting points.");
    int index = 1;
    bool outOfCandidates = false;
    for (auto& point : createdCity->points)
    {
        while (!point->isFull())
        {
            CityFunctions::connectTwoPoints(point, nullptr, createdCity, outOfCandidates);
            if (outOfCandidates)
                break;
        }
        index++;
    }
    LOG("Finished connecting points.");

    return createdCity;
}

std::string CityFunctions::getRandomName(std::vector<std::string>* names)
{
    if (names->empty())
    {
        throw std::runtime_error("No names available to select.");
    }

    std::uniform_int_distribution<> distrib(0, names->size() - 1);
    
    int nameIndex = distrib(gen);
    std::string streetName = (*names)[nameIndex];
    names->erase(names->begin() + nameIndex);
    return streetName;
}

std::vector<std::string> CityFunctions::readFile(const std::string& path)
{
    std::vector<std::string> names;
    std::ifstream file(path);
    std::string line;
    while(std::getline(file, line))
    {
        names.emplace_back(line); 
    }
    file.close();
    return names;
}

void CityFunctions::disconnectTwoPoints(ConnectionPoint* point1, ConnectionPoint* point2, City* city)
{
    Street* sharedStreet = point1->getSharedStreet(point2);

    if (sharedStreet == nullptr)
    {
        LOG("[SHARED STREET NOT FOUND] between " << point1->name << " and " << point2->name << " shared street isnt found.");
        return;
    }

    point1->removeStreet(sharedStreet);
    point2->removeStreet(sharedStreet);

    city->deleteStreet(sharedStreet);

    point1->removePoint(point2);
    point2->removePoint(point1);
}

bool CityFunctions::connectTwoPoints(ConnectionPoint* point1, ConnectionPoint* point2, City* city, bool& outOfCandidates)
{
    point2 = (point2 == nullptr) ? CityFunctions::getValidPoint(city, point1, true, false) : point2;

    if (point2 == nullptr)
    {
        outOfCandidates = true;
        return false;
    }

    bool isAlreadyIn = false;
    bool p1_Success = point1->addPoint(point2, isAlreadyIn);

    if (p1_Success && !isAlreadyIn)
    {
        bool p2_Success = point2->addPoint(point1, isAlreadyIn);
        if (p2_Success && !isAlreadyIn)
        {
            Street* str = new Street();

            city->addStreet(str);

            point1->addStreet(str);
            point2->addStreet(str);

            str->addPoint(point1, BACK);
            str->addPoint(point2, FRONT);

            return true;
        }
        else
        {
            point1->removePoint(point2);
            point2->removePoint(point1);
        }
    }
    else if (!p1_Success && !isAlreadyIn)
    {
        point1->removePoint(point2);
        return false;
    }
    else
    {
        return false;
    }
}

ConnectionPoint* CityFunctions::getValidPoint(City* city, ConnectionPoint* dontMatch, bool checkIsFull, bool checkNoSpaceAround)
{
    if (candidatePoints.empty())
    {
        DEBUG_LOG("Getting candidates...");
        for (auto& point : city->points)
        {
            if (point == dontMatch)
                continue;
            if (checkIsFull && point->isFull())
                continue;
            if (checkNoSpaceAround && point->noSpaceAround)
                continue;
            candidatePoints.emplace_back(point);
        }
        DEBUG_LOG("Finished getting candidates.");
    }

    if (candidatePoints.empty())
    {
        LOG("Out of candidates.");
        return nullptr;
    }

    return CityFunctions::getRandomCandidate(dontMatch);
}

ConnectionPoint* CityFunctions::getRandomCandidate(ConnectionPoint* dontMatch)
{
    std::uniform_int_distribution<> distrib(0, candidatePoints.size() - 1);
    ConnectionPoint* candidate = candidatePoints[distrib(gen)];

    if (candidatePoints.size() == 1 && candidatePoints[0] == dontMatch)
    {
        DEBUG_LOG("Candidate vector had only one element.");
        candidatePoints.clear();
        DEBUG_LOG("Candidate vector cleared.");
        return nullptr;
    }
    else if (candidatePoints.size() == 1)
    {
        return candidatePoints[0];
    }
    else
    {
        DEBUG_LOG("Getting random candidate...");
        while (candidate == dontMatch)
        {
            candidate = candidatePoints[distrib(gen)];
            if (candidate != dontMatch)
                return candidate;
        }
        DEBUG_LOG("Finished candidate search.");

        return candidate;
    }
}

void CityFunctions::removeFromCandidates(ConnectionPoint* point)
{
    auto it = std::find(candidatePoints.begin(), candidatePoints.end(), point);
    if (it != candidatePoints.end())
    {
        DEBUG_LOG("[CANDIDATE REMOVAL] " << point->name << " was removed from candidates.");
        candidatePoints.erase(it);
    }
    else
    {
        DEBUG_LOG("[CANDIDATE REMOVAL NOT FOUND] " << point->name << " was not found in candidatePoints.");
    }
}

Street* CityFunctions::getSharedStreet(ConnectionPoint* point1, ConnectionPoint* point2)
{
    for (auto& str : point1->connectedStreets)
    {
        if ((str->backPoint == point1 && str->frontPoint == point2) || (str->backPoint == point2 && str->frontPoint == point1))
            return str;
    }
    return nullptr;
}

void City::removeUnusedStreets()
{
    for (auto it = this->streets.begin(); it != this->streets.end(); ) {
        Street* street = *it;
        if (street->backPoint == nullptr && street->frontPoint == nullptr)
        {
            delete street;
            it = this->streets.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

ConnectionPoint* City::getPointByName(const std::string& name)
{
    for (auto& point : this->points)
    {
        if (point->name == name)
            return point;
    }
    return nullptr;
}

ConnectionPoint* City::getRandomPoint(int beginIndex)
{
    beginIndex = (beginIndex == -1) ? this->points.size() / 2 : beginIndex;
    std::uniform_int_distribution<> randomPointInd(beginIndex, this->points.size() - 1);
    return this->points[randomPointInd(gen)];
}

ConnectionPoint* City::getLastCreatedPoint()
{
    ConnectionPoint* point = this->points.back();
    if (point->connectedStreets.size() >= point->maxConnection)
        point->maxConnection += 1;
    return point;
}

std::vector<ConnectionPoint*> City::getShortestPath(ConnectionPoint* base, ConnectionPoint* destination)
{
    std::vector<ConnectionPoint*> visited = std::vector<ConnectionPoint*>();
    
    ConnectionPoint* smallest = nullptr;

    this->turnStreetsIntoVectors(destination);
    //this->printStreets(false);

    do
    {
        visited.emplace_back(base);

        if (base == destination)
            break;

        for (auto& connectedPoints : base->connectedPoints)
        {
            Street* sharedStreet = CityFunctions::getSharedStreet(connectedPoints, base);

            if (/*sharedStreet == nullptr || */(sharedStreet->goBack && sharedStreet->backPoint != connectedPoints) || (sharedStreet->goFront && sharedStreet->frontPoint != connectedPoints))
                continue;

            if (connectedPoints->cost == 0)
                connectedPoints->calculateCost(sharedStreet->weight);

            if (smallest == nullptr)
            {
                smallest = connectedPoints;
                continue;
            }
            else if (smallest->cost > connectedPoints->cost)
            {
                smallest = connectedPoints;
            }
        }
        base = smallest;
        smallest = nullptr;

    } while (true);

    this->turnStreetsIntoLines();
    this->flipVisited();
    return visited;
}

bool City::isPointExist(const std::string& name)
{
    for (auto& point : this->points)
    {
        if (name == point->name)
            return true;
    }
    return false;
}

void City::flipVisited()
{
    for (auto& point : this->points)
    {
        point->isVisited = false;
        point->cost = 0;
    }
}

void City::turnStreetsIntoLines()
{
    for (auto& str : this->streets)
    {
        str->goBack = false;
        str->goFront = false;
    }
}

void City::turnStreetsIntoVectors(ConnectionPoint* destination)
{
    std::vector<ConnectionPoint*> nextITpoints = {destination};
    std::vector<ConnectionPoint*> currentITpoints = {};

    do
    {
        currentITpoints = nextITpoints;
        nextITpoints.clear();
        for (auto& point : currentITpoints)
        {
            for (auto& str : point->connectedStreets)
            {

                if (!str->goBack && !str->goFront) // this ensure that the street is untouched
                {
                    if (str->backPoint == point)
                    {
                        str->goBack = true;
                        nextITpoints.emplace_back(str->frontPoint);
                    }
                    else if (str->frontPoint == point)
                    {
                        str->goFront = true;
                        nextITpoints.emplace_back(str->backPoint);
                    }
                }
            }
        }
        currentITpoints.clear();
    } while (!nextITpoints.empty());
}

void City::countMoreThan2ConnectionStreets()
{
    for (auto& p : this->points)
    {
        for (auto& conStr : p->connectedStreets)
        {
            conStr->appeared += 1;
        }
    }

    int count = 0;
    for (auto& str : this->streets)
    {
        if (str->appeared > 2)
            count++;
    }
    LOG("Number of streets that has more than two connections: " << count);
}

void City::countDisplayedPoints()
{
    int i = 0;
    for (auto& point : this->points)
    {
        if (point->isDisplayed())
            i++;
    }
    LOG("displayed points count: " << i);
}

void City::printPoints(bool details, bool relatedPoints)
{
    int i = 1;
    std::cout << "POINTS------------------------------------" << std::endl;
    for (auto& point : this->points)
    {
        std::cout << i << ". " << point->name << std::endl;

        if (relatedPoints)
        {
            std::cout << "\tRelated streets: " << std::endl;
            for (auto& str : point->connectedStreets)
            {
                std::cout << "\t\t" << str->name << std::endl;
            }
            std::cout << "\tRelated points: " << std::endl;
            for (auto& point : point->connectedPoints)
            {
                std::cout << "\t\t" << point->name << std::endl;
            }
        }

        if (details)
        {
            std::cout << "\tmax connections: " << (int)point->maxConnection << std::endl;
            std::cout << "\tweight: " << (int)point->weight << std::endl;
            std::cout << "\tcost: " << (int)point->cost << std::endl;
        }

        i++;
    }
    std::cout << "POINTS------------------------------------" << std::endl;
}

void City::printStreets(bool details)
{
    std::cout << "STREETS------------------------------------" << std::endl;
    int i = 1;
    for (auto& str : this->streets)
    {
        std::cout << i << ". " << str->name << std::endl;
        std::cout << "\tgoFront: " << str->goFront << " goBack: " << str->goBack << std::endl;
        if (details)
        {
            std::cout << "\t" << (int)str->weight << std::endl;
            
            if (str->backPoint != nullptr)
                std::cout << "\tback point: " << str->backPoint->name << std::endl;
            if (str->frontPoint != nullptr)
                std::cout << "\tfront point: " << str->frontPoint->name << std::endl;
        }
        i++;
    }
}

ConnectionPoint::ConnectionPoint()
{
    this->connectedStreets = std::vector<Street*>();
    this->connectedPoints = std::vector<ConnectionPoint*>();
    this->name = CityFunctions::getRandomName(&CityFunctions::pointNames);
    this->cost = 0;
    this->isVisited = false;
    this->noSpaceAround = false;
    this->shape = new sf::Sprite();

    std::uniform_int_distribution<> randomWeight(1, 9);
    this->weight = randomWeight(gen);

    std::uniform_int_distribution<> randomConnectionCount(2, 3);
    this->maxConnection = randomConnectionCount(gen);
}

ConnectionPoint::~ConnectionPoint()
{
    for (auto& str : this->connectedStreets)
    {
        delete str;
    }
    this->connectedStreets.clear();
}

void ConnectionPoint::calculateCost(uint8_t streetWeight)
{
    this->cost = this->weight + streetWeight;
}

bool ConnectionPoint::addStreet(Street* street)
{
    if (!this->isStreetConnected(street))
    {
        this->connectedStreets.emplace_back(street);
        return true;
    }
    else
    {
        DEBUG_LOG("[ALREADY CONNECTED] " << street->name << " was already connected to " << this->name);
        return false;
    }
}

void Street::addPoint(ConnectionPoint* point, Sides side)
{
    if (side == BACK)
        this->backPoint = point;
    else
        this->frontPoint = point;
}

bool Street::removePoint(ConnectionPoint* point)
{
    if (point == this->backPoint)
    {
        this->backPoint = nullptr;
        return true;
    }
    else if (point == this->frontPoint)
    {
        this->frontPoint = nullptr;
        return true;
    }
    else
        return false;
}

bool City::isValidConnectionPoint(ConnectionPoint* point) 
{
    if (point == nullptr)
        return false;

    auto& points = this->points;
    if (std::find(points.begin(), points.end(), point) == points.end())
    {
        return false;
    }

    auto& toBeMoved = this->toBeDisplayedPoints;
    if (std::find(toBeMoved.begin(), toBeMoved.end(), point) != toBeMoved.end())
    {
        return false;
    }

    return true;
}

void CityFunctions::clearRoute()
{
    for (auto str : CityFunctions::route)
    {
        delete str;
    }
    CityFunctions::route.clear();
}

bool ConnectionPoint::isPointAlreadyConnected(ConnectionPoint* point)
{
    for (auto& p : this->connectedPoints)
    {
        if (point == p)
            return true;
    }
    return false;
}

bool ConnectionPoint::isFull() const
{
    return (int)this->connectedPoints.size() >= (int)this->maxConnection;
}

bool Object::isDisplayed() const
{
    if (this->shape->getTexture() == nullptr)
        return false;
    else
        return true;
}

Street* ConnectionPoint::getSharedStreet(ConnectionPoint* point)
{
    for (auto& str : this->connectedStreets)
    {
        if (str->backPoint == point)
        {
            return str;
        }
        if (str->frontPoint == point)
        {
            return str;
        }
    }
    return nullptr;
}
