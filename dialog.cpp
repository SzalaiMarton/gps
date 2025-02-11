#include "dialog.h"

string Dialog::nextDialog(int next)
{
    if (next == 5)
    {
        return root_text.text;
    }
    else
    {
        return root_text.nextTexts[next]->text;
    }
}

string Dialog::writeText(int next)
{
    string response;
    cout << Dialog::nextDialog(next);
    if (!(next >= root_text.nextTexts.size()))
    {
        cout << endl;
        return "";
    }
    else
    {
        cin >> response;
        return response;
    }
}

void Dialog::loadDialog()
{
    Dialog::Text createStreetName("Name the street: ", true);
    Dialog::Text createStreetLength("Define the length: ", true);
    Dialog::Text createCity("Define the population size: ", true);
    Dialog::Text cityMissing("You haven't created a city yet. First you have to create a city to create a street.", false);

    createStreetName.nextTexts.push_back(&createStreetLength);
    root_text.nextTexts.push_back(&createStreetName);
    root_text.nextTexts.push_back(&createCity);
    root_text.nextTexts.push_back(&cityMissing);
}