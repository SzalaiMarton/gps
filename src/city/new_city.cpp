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

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 9);

    this->weight = distrib(gen);
}

Street::~Street()
{
    if (this->backPoint != nullptr)
        this->backPoint->removeConnection(this);
    if(this->frontPoint != nullptr)
            this->frontPoint->removeConnection(this);
}

bool ConnectionPoint::isStreetConnected(Street* street)
{
    for (auto& str : this->connections)
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
        if (point->connections.size() < point->maxConnection)
        {
            createdCity->connectPointToRandomPoint(point);
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

ConnectionPoint* City::getRandomUnfinishedPoint(ConnectionPoint* targetPoint)
{
    for (auto& point : this->points)
    {
        if (point != targetPoint && point->connections.size() < point->maxConnection)
            return point;
    }
    return nullptr;
}

void City::connectPointToRandomPoint(ConnectionPoint* point)
{
    ConnectionPoint* unfinishedPoint = this->getRandomUnfinishedPoint(point);

    if (unfinishedPoint == nullptr)
    {
        std::cout << "out of unfinished points" << std::endl;
        return;
    }

    Street* connectionLine = new Street();
    this->addStreet(connectionLine);

    point->connectStreet(connectionLine, BACK);
    unfinishedPoint->connectStreet(connectionLine, FRONT);
}

std::vector<ConnectionPoint*> City::getShortestPath(const std::string& base, const std::string& destination)
{
    std::vector<ConnectionPoint*> visited = std::vector<ConnectionPoint*>();
    
    ConnectionPoint* basePoint = this->getPointByName(base);
    ConnectionPoint* currentPoint = nullptr;
    ConnectionPoint* smallest = nullptr;

    if (basePoint == nullptr)
    {
        std::cout << "starting point doesn't exist" << std::endl;
        return visited;
    }

    if (!this->isPointExist(destination))
    {
        std::cout << "destination point doesn't exist" << std::endl;
        return visited;
    }

    int iterationCount = 0;

    do
    {
        if (basePoint == nullptr)
        {
            std::cout << "base point was nullptr" << std::endl;
            break;
        }

        visited.emplace_back(basePoint);
        basePoint->isVisited = true;

        if (basePoint != nullptr && basePoint->name == destination)
        {
            std::cout << "destination reached" << std::endl;
            break;
        }

        for (auto& str : basePoint->connections)
        {
            if (str->backPoint != nullptr && str->backPoint != basePoint)
                currentPoint = str->backPoint;
            else if (str->frontPoint != nullptr && str->frontPoint != basePoint)
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

        iterationCount++;

        std::cout << iterationCount << "." << std::endl;

    } while (true);

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

void City::printPoints(bool details)
{
    int i = 1;
    std::cout << "POINTS------------------------------------" << std::endl;
    for (auto& point : this->points)
    {
        std::cout << i << ". " << point->name << std::endl;

        if (details)
        {
            std::cout << "\tmax connections: " << (int)point->maxConnection << std::endl;
            std::cout << "\tweight: " << (int)point->weight << std::endl;
            std::cout << "\tcost: " << (int)point->cost << std::endl;
            if (!point->connections.empty())
            {
                std::cout << "\tConnections:" << std::endl;
                for (auto& str : point->connections)
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
    for (auto& str : this->streets)
    {
        std::cout << str->name << std::endl;
        std::cout << "\t" << (int)str->weight << std::endl;
        if (str->backPoint != nullptr)
            std::cout << "\tback point: " << str->backPoint->name << std::endl;
        if (str->frontPoint != nullptr)
            std::cout << "\tfront point: " << str->frontPoint->name << std::endl;
    }
}

ConnectionPoint::ConnectionPoint()
{
    this->connections = std::vector<Street*>();
    this->name = CityFunctions::getRandomName(CityFunctions::pointNames);
    this->cost = 0;
    this->isVisited = false;

    static std::random_device rd;
    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> randomWeight(1, 9);
    this->weight = randomWeight(gen);

    std::uniform_int_distribution<> randomConnectionCount(1, 3);
    this->maxConnection = randomConnectionCount(gen);
}

void ConnectionPoint::removeConnection(Street* street)
{
    if (this->isStreetConnected(street))
    {
        for (int i = 0; i < this->connections.size(); i++)
        {
            if (this->connections[i] == street)
            {
                this->connections.erase(this->connections.begin() + i);

                if (street->backPoint == this)
                    street->backPoint = nullptr;
                else 
                    street->frontPoint = nullptr;

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
    if (this->connections.size() < this->maxConnection)
    {
        this->connections.emplace_back(street);

        if (side == BACK)
            street->backPoint = this;
        else if (side == FRONT)
            street->frontPoint = this;
    }
    else
    {
        std::cout << "connections already full for " << this->name << std::endl;
    }
}