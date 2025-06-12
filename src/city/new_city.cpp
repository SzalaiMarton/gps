#include "new_city.h"
#include "../display/display.h"

Street::Street(const std::string& name)
{
    this->changeName(name);
    this->connectedStreetsBack = std::vector<Street*>();
    this->connectedStreetsFront = std::vector<Street*>();
    this->isBackFull = false;
    this->isFrontFull = false;
    this->isRoot = false;
}

Street::Street(const std::string& name, std::vector<Street*>& back, std::vector<Street*>& front)
{
    this->changeName(name);
    this->connectedStreetsBack = back;
    this->connectedStreetsFront = front;
    this->isBackFull = false;
    this->isFrontFull = false;
    this->isRoot = false;
}

void Street::checkIfBackFull()
{
    if (this->connectedStreetsBack.size() == this->maxBackStreetsSize)
        this->isBackFull = true;
    else
        this->isBackFull = false;
}

void Street::checkIfFrontFull()
{
    if (this->connectedStreetsFront.size() == this->maxFrontStreetsSize)
        this->isFrontFull = true;
    else
        this->isFrontFull = false;
}

void Street::addStreetToBack(Street *street, bool addToTheOther)
{
    if (street == nullptr)
        return;
    
    if (addToTheOther)
        street->addStreetToFront(this, false);

    this->checkIfBackFull();

    if (!this->isBackFull && !this->isStreetConnected(street))
    {
        this->connectedStreetsBack.emplace_back(street);
        if (this->connectedStreetsBack.size() >= this->maxBackStreetsSize)
            this->isBackFull = true;
    }
}

void Street::addStreetToFront(Street *street, bool addToTheOther)
{
    if (street == nullptr)
        return;

    if (addToTheOther)
        street->addStreetToBack(this, false);

    this->checkIfFrontFull();

    if (!this->isFrontFull && !this->isStreetConnected(street))
    {
        this->connectedStreetsFront.emplace_back(street);
        if (this->connectedStreetsFront.size() >= this->maxFrontStreetsSize)
            this->isFrontFull = true;
    }
}

void Street::addStreetToNewNeighbors(std::vector<Street*>& holderForStreetsBack, std::vector<Street*>& holderForStreetsFront)
{
    if (!holderForStreetsBack.empty())
    {
        this->connectedStreetsBack.clear();

        for (auto& str : holderForStreetsBack)
        {
            this->addStreetToBack(str, true);
        }
    }

    if (!holderForStreetsFront.empty())
    {
        this->connectedStreetsFront.clear();
       
        for (auto& str : holderForStreetsFront)
        {
            this->addStreetToFront(str, true);
        }
    }
}

void Street::removeFromBack(Street* street, bool removeTheOther)
{
    for (int index = 0; index < this->connectedStreetsBack.size(); index++)
    {
        if (this->connectedStreetsBack[index] == street)
            this->connectedStreetsBack.erase(this->connectedStreetsBack.begin() + index);
    }

    if (removeTheOther)
        street->removeFromFront(this, false);
}

void Street::removeFromFront(Street* street, bool removeTheOther)
{
    for (int index = 0; index < this->connectedStreetsFront.size(); index++)
    {
        if (this->connectedStreetsFront[index] == street)
            this->connectedStreetsFront.erase(this->connectedStreetsFront.begin() + index);
    }

    if (removeTheOther)
        street->removeFromBack(this, false);
}

void Street::removeStreetFromNeighbors(std::vector<Street*>& holderForStreetsBack, std::vector<Street*>& holderForStreetsFront)
{
    for (auto& str : this->connectedStreetsBack)
    {
        holderForStreetsBack.emplace_back(str);
        str->removeFromBack(this, true);
    }
    for (auto& str : this->connectedStreetsFront)
    {
        holderForStreetsFront.emplace_back(str);
        str->removeFromBack(this, true);
    }

    this->isDisplayed = false;

    this->connectedStreetsBack.clear();
    this->connectedStreetsFront.clear();
}

void Street::changeTexture(sf::Texture* texture)
{
    if (texture == nullptr)
    {
        return;
    }
    this->shape.setTexture(*texture);
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

void Street::changeName(const std::string& name)
{
    if (name.length() <= 30)
    {
        this->name = name;
    }
}

CityFunctions::VectorSide Street::getEmptyVectorSide()
{
    if (!this->isBackFull)
        return CityFunctions::VECTOR_BACK;
    else if (!this->isFrontFull)
        return CityFunctions::VECTOR_FRONT;
    else
        return CityFunctions::VECTOR_INVALID_SIDE;
}

City::City(const std::string& name)
{
    this->changeName(name);
    this->streets = std::vector<Street*>();
}

City::City(const std::string& name, std::vector<Street*> streets)
{
    this->changeName(name);
    this->streets = streets;
}

int City::getUnfinishedCount()
{
    return (int)this->unfinishedStreets.size();
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
        return;
    }
    this->shape.setTexture(*texture);
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

void City::removeUnfinishedStreet(Street *street)
{
    if (street == nullptr)
    {
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
        std::cout << displayIndex + 1 << " " << this->streets[index]->name << std::endl;
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

    while(resCity->streets.size() != maxStreetCount)
    {
        Street* street = new Street(CityFunctions::getRandomName(randomNames));
        resCity->addStreet(street);
        resCity->addUnfinishedStreet(street);
    }

    resCity->connectStreets();

    //resCity.printUnfinished();
    resCity->printStreets(true);

    Display::displayCity(resCity);

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

Street* CityFunctions::getWhereToConnectStreet(Street* street)
{
    for (const auto& str : street->connectedStreetsBack)
    {
        if (str->isDisplayed)
        {
            return str;
        }
    }
    for (const auto& str : street->connectedStreetsFront)
    {
        if (str->isDisplayed)
        {
            return str;
        }
    }
    return nullptr;
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
    return city->rootStreet;
}

void City::connectStreets()
{
    Street* connectTo = this->rootStreet;
    CityFunctions::VectorSide vectorSide{};

    for (int index = 0; index < this->unfinishedStreets.size(); index++)
    {
        if (this->unfinishedStreets[index] == connectTo)
            continue;

        vectorSide = connectTo->getEmptyVectorSide();

        if (vectorSide == CityFunctions::VECTOR_BACK)
        {
            connectTo->addStreetToBack(this->unfinishedStreets[index], true);
            for (auto& strOuter : connectTo->connectedStreetsBack)
            {
                for (auto& str : connectTo->connectedStreetsBack)
                {
                    if (str == strOuter)
                        continue;

                    strOuter->addStreetToFront(str, false);
                }
            }
        }
        else if (vectorSide == CityFunctions::VECTOR_FRONT)
        {
            connectTo->addStreetToFront(this->unfinishedStreets[index], true);
            for (auto& strOuter : connectTo->connectedStreetsFront)
            {
                for (auto& str : connectTo->connectedStreetsFront)
                {
                    if (str == strOuter)
                        continue;

                    strOuter->addStreetToFront(str, false);
                }
            }
        }
        else
        {
            connectTo->isFinished = true;
            connectTo = this->getFirstUnfinishedStreet();
            if (connectTo == nullptr)
            {
                std::cerr << "nullptr at City::connectStreets()" << std::endl;
                return;
            }
            index--;
        }
    }
}

void City::connectToRandomUnfinishedStreet(Street* street)
{
    Street* randomUnfinished = this->getMiddleUnfinishedStreet();

    randomUnfinished->addStreetToBack(street, true);
}

CityFunctions::VectorSide CityFunctions::getWhereToSnap(Street* target, Street* connectTo)
{
    for (const auto& el : connectTo->connectedStreetsBack)
    {
        if (el == target) 
            return CityFunctions::VECTOR_BACK;
    }
    for (const auto& el : connectTo->connectedStreetsFront)
    {
        if (el == target) 
            return CityFunctions::VECTOR_FRONT;
    }
    return CityFunctions::VECTOR_INVALID_SIDE;
}
