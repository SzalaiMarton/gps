#include "dialog.h"
#include "city.h"

int Dialog::processInput(string response)
{
    if (response == "1")
    {
        if (CityParts::cityVector.empty())
        {
            return Dialog::CITY_ERROR_MESSAGE;
        }
        else
        {
            return Dialog::CREATE_STREET_INDEX;
        }
    }
    else if (response == "2")
    {
        return Dialog::CREATE_CITY_INDEX;
    }
    else if (response == "exit")
    {
        return -1;
    }
    else
    {
        return 100;
    }
}

void Dialog::printListElementsToConsole(int currentDialogIndex)
{
    switch (currentDialogIndex)
    {
    case Dialog::CREATE_STREET_INDEX:
        Dialog::printStreets();
        break;
    case Dialog::CREATE_CITY_INDEX:
        Dialog::printCities();
        break;
    default:
        cout << "Cannot list without index" << endl;
        return;
    }
}

void Dialog::printStreets()
{
    for (auto el : CityParts::streetVector)
    {
        cout << el.street_name << endl;
    }
}

void Dialog::printCities()
{
    for (auto el : CityParts::cityVector)
    {
        cout << el.cityName << endl;
    }
}

void Dialog::createObject(Dialog::Text* currentDialog, int currentDialogIndex)
{
    vector<string> response;
    for (auto item : currentDialog->nextTexts)
    {
        response.push_back(Dialog::writeText(item, currentDialogIndex));
    }
    if(currentDialogIndex == Dialog::CREATE_STREET_INDEX)
    {
        CityParts::createStreet(response);
    }
    else if(currentDialogIndex == Dialog::CREATE_CITY_INDEX)
    {
        CityParts::createCity(response);
    }
}