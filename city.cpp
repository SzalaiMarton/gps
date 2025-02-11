#include "city.h"

void CityParts::createStreet(string name, int length, vector<CityParts::Street*> connectedStreetB, vector<CityParts::Street*> connectedStreetF)
{
    CityParts::Street newStreet(name,length);
    newStreet.connected_streets_back = connectedStreetB;
    newStreet.connected_streets_front = connectedStreetF;
}