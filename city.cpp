#include "city.h"

vector<CityParts::City> CityParts::cityVector;
vector<CityParts::Street> CityParts::streetVector;

void CityParts::createStreet(vector<string> properties)
{
    CityParts::Street* temp = new CityParts::Street(properties[0], stoi(properties[1])); // [0]-name, [1]-length
    streetVector.push_back(*temp);
    delete[] temp;
}

void CityParts::createCity(vector<string> properties)
{
    CityParts::City* temp = new CityParts::City(properties[0], stoi(properties[1])); // [0]-name, [1]-density in number
    cityVector.push_back(*temp);
    delete[] temp;
}