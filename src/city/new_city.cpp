#include "new_city.h"
#include <iostream>
#include <cstdint>
#include <random>
#include <fstream>
#include <sstream>

namespace CityFunctions
{
    std::vector<std::string> pointNames = CityFunctions::readFile("names/point_names.txt");
    std::vector<std::string> streetNames = CityFunctions::readFile("names/street_names.txt");
}

Street::Street()
{
    this->name = CityFunctions::getRandomName(CityFunctions::streetNames);
    this->backPoint = nullptr;
    this->frontPoint = nullptr;
    this->goFront = false;
    this->goBack = false;
    this->isDisplayed = false;
    this->shape = new sf::Sprite();

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 9);

    this->weight = distrib(gen);
}

Street::~Street()
{
    if (this->backPoint != nullptr)
        this->backPoint->removeStreet(this);
    if(this->frontPoint != nullptr)
        this->frontPoint->removeStreet(this);
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

void City::removeStreet(Street* street)
{
    delete street;
}

void City::addPoint(ConnectionPoint* point)
{
    if (point != nullptr)
        this->points.emplace_back(point);
}

City* CityFunctions::generateCity(const std::string& name, int pointCount)
{
    City* createdCity = new City(name);

    for (int i = 0; i < pointCount; i++)
    {
        createdCity->addPoint(new ConnectionPoint());
    }

    for (auto& point : createdCity->points)
    {
        if (point->connectedStreets.size() < point->maxConnection)
        {
            createdCity->connectTwoPoints(point);
        }
    }

    return createdCity;
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

ConnectionPoint* City::getPointByName(const std::string& name)
{
    for (auto& point : this->points)
    {
        if (point->name == name)
            return point;
    }
    return nullptr;
}

ConnectionPoint* City::getRandomPoint()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> randomPointInd(this->points.size() / 2, this->points.size() - 1);

    return this->points[randomPointInd(gen)];
}

ConnectionPoint* City::getLastCreatedPoint()
{
    ConnectionPoint* point = this->points.back();
    if (point->connectedStreets.size() >= point->maxConnection)
        point->maxConnection += 1;
    return point;
}

void City::connectTwoPoints(ConnectionPoint* point1, ConnectionPoint* point2)
{
    if (point2 != nullptr)
    {
        point1->connectPoint(point2, this);
        return;
    }

    for (auto& point : this->points)
    {
        if (point1->isPointAlreadyConnected(point) || point->connectedPoints.size() >= point->maxConnection || point == point1 || point->noSpaceAround)
            continue;

        point1->connectPoint(point, this);
        return;
    }
    std::cout << "couldn't connect " << point1->name << " to nowhere" << std::endl;
}

std::vector<ConnectionPoint*> City::getShortestPath(const std::string& base, const std::string& destination)
{
    std::vector<ConnectionPoint*> visited = std::vector<ConnectionPoint*>();
    
    ConnectionPoint* basePoint = this->getPointByName(base);
    ConnectionPoint* destinationPoint = this->getPointByName(destination);
    ConnectionPoint* currentPoint = nullptr;
    ConnectionPoint* smallest = nullptr;

    if (basePoint == nullptr)
    {
        std::cout << "starting point doesn't exist" << std::endl;
        return visited;
    }

    if (destinationPoint == nullptr)
    {
        std::cout << "destination point doesn't exist" << std::endl;
        return visited;
    }

    this->turnStreetsIntoVectors(destinationPoint);
    //this->printStreets(false);

    do
    {
        if (basePoint == nullptr)
        {
            std::cout << "base point was nullptr" << std::endl;
            break;
        }

        visited.emplace_back(basePoint);
        basePoint->isVisited = true;

        if (basePoint != nullptr && basePoint == destinationPoint)
        {
            std::cout << "destination reached" << std::endl;
            break;
        }

        for (auto& str : basePoint->connectedStreets)
        {
            if (str->backPoint != nullptr && str->backPoint != basePoint && str->goBack)
                currentPoint = str->backPoint;
            else if (str->frontPoint != nullptr && str->frontPoint != basePoint && str->goFront)
                currentPoint = str->frontPoint;

            if (smallest == nullptr && currentPoint != nullptr && !currentPoint->isVisited)
            {
                smallest = currentPoint;
                smallest->calculateCost(str->weight);
                continue;
            }

            if (currentPoint != nullptr && smallest != nullptr)
            {
                currentPoint->calculateCost(str->weight);
                if (smallest->cost > currentPoint->cost && !currentPoint->isVisited)
                    smallest = currentPoint;
            }
        }

        basePoint = smallest;
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
    // dont need check -> this function is always in getShortestRoute where base and destination already checked
    // this function goes from destination to every street and flip goFront or goBack
    // or another approach: it goes through all the points from destination and every point stores which of its neighbour is closer to destination
    //      - create another function for this to see which one is faster

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
            if (!point->connectedStreets.empty())
            {
                std::cout << "\tConnections:" << std::endl;
                for (auto& str : point->connectedStreets)
                {
                    std::cout << "\t\t- " << str->name << std::endl;
                }
            }
        }

        i++;
    }
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
    this->name = CityFunctions::getRandomName(CityFunctions::pointNames);
    this->cost = 0;
    this->isVisited = false;
    this->isDisplayed = false;
    this->noSpaceAround = false;
    this->shape = new sf::Sprite();

    static std::random_device rd;
    static std::mt19937 gen(rd());

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

void ConnectionPoint::removeRelatedPoint(ConnectionPoint* toBeRemoved, City* city)
{
    Street* sharedStr = this->getSharedStreet(toBeRemoved);
    this->removeStreet(sharedStr);
    toBeRemoved->removeStreet(sharedStr);

    this->removePoint(toBeRemoved);
    toBeRemoved->removePoint(this);

    this->maxConnection--;
    this->noSpaceAround = true;
}

void ConnectionPoint::removeStreet(Street* street)
{
    if (this->isStreetConnected(street))
    {
        for (int i = 0; i < this->connectedStreets.size(); i++)
        {
            if (this->connectedStreets[i] == street)
            {
                this->connectedStreets.erase(this->connectedStreets.begin() + i);
                return;
            }
        }
    }
}

void ConnectionPoint::removePoint(ConnectionPoint* point)
{
    if (this->isPointAlreadyConnected(point))
    {
        for (int i = 0; i < this->connectedPoints.size(); i++)
        {
            if (this->connectedPoints[i] == point)
            {
                this->connectedPoints.erase(this->connectedPoints.begin() + i);
                return;
            }
        }
    }
}

void ConnectionPoint::calculateCost(uint8_t streetWeight)
{
    this->cost = this->weight + streetWeight;
}

void ConnectionPoint::connectStreet(Street* street, Sides side)
{
    this->connectedStreets.emplace_back(street);

    if (side == BACK)
        street->backPoint = this;
    else if (side == FRONT)
        street->frontPoint = this;
}

void ConnectionPoint::connectPoint(ConnectionPoint* point, City* city)
{
    if (this->connectedStreets.size() >= this->maxConnection || point->connectedStreets.size() >= point->maxConnection)
    {
        std::cout << "connections already full for " << this->name << " or " << point->name << std::endl;
        return;
    }

    Street* str = new Street();
    city->addStreet(str);

    this->connectStreet(str, BACK);
    point->connectStreet(str, FRONT);

    this->connectedPoints.emplace_back(point);
    point->connectedPoints.emplace_back(this);
}

bool ConnectionPoint::isPointAlreadyConnected(ConnectionPoint* point)
{
    for (auto& str : this->connectedPoints)
    {
        if (point == str)
            return true;
    }
    return false;
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
