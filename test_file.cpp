#include "dialog.h"
#include "commands.h"
#include "city.h"

CityParts city;

int test_generateCityForWeb()
{
    string name = "Berlin";
    city.generateCityForWeb(name, 100, 4);
    if (city.cityVector.size() > 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}



int main()
{
    cout << "test started." << endl;
    cout << test_generateCityForWeb() << endl;
    return 0;
}