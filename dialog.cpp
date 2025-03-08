#include "dialog.h"
#include "city.h"

Dialog dialog;

Dialog::Command MainHelp(dialog.HELP_COMMAND);
Dialog::Command Create(dialog.CREATE_COMMAND);
Dialog::Command Edit(dialog.EDIT_COMMAND);

void Dialog::initializeCommands()
{
    MainHelp.loadHelpMessage(loadHelpFromFile(dialog.PATH_TO_HELP));
    Create.loadHelpMessage(loadHelpFromFile(dialog.PATH_TO_CREATE));
    Edit.loadHelpMessage(loadHelpFromFile(dialog.PATH_TO_EDIT));
}

string Dialog::loadHelpFromFile(string path)
{
    ifstream file(path);
    string reValue = "";
    string text;
    while(getline(file, text))
    {
        reValue += text + "\n"; 
    }
    file.close();
    return reValue;
}

void Dialog::processInput(vector<string> input)
{
    //handling help commands
    for (string el : input)
    {
        if(el == "-h")
        {
            handleHelp(input[0]);
            return;
        }
    }
    handleCommands(input);
}

void Dialog::handleHelp(string input)
{
    if(input == MainHelp.commandName) {MainHelp.printHelp();}
    else if(input == Create.commandName) {Create.printHelp();}
    else if(input == Edit.commandName) {Edit.printHelp();}
}

void Dialog::handleCommands(vector<string> input)
{
    if (input[0] == Create.commandName)
    {
        input.erase(input.begin());
        if (input.size() == 0)
        {
            Dialog::errorHandle(Create.commandName);
            return;
        }
        Dialog::handleCreateObjects(input);
    }
    else if (input[0] == Edit.commandName)
    {
        input.erase(input.begin());
        if (input.size() == 0)
        {
            Dialog::errorHandle(Edit.commandName);
            return;
        }
        Dialog::handleEditObjects(input);
    }
}

void Dialog::handleCreateObjects(vector<string> properties)
{
    if (properties[0] == "street")
    {
        CityParts::createStreet(properties);
    }
    else if (properties[0] == "city")
    {
        CityParts::createCity(properties);
    }
}

void Dialog::handleEditObjects(vector<string> properties)
{
    string input;
    if (properties[0] == "street")
    {
        Dialog::listAllStreets();
    }
    else if (properties[0] == "city")
    {
        Dialog::listAllCities();
    }
}

void Dialog::errorHandle(string commandName)
{
    if (commandName == Create.commandName)
    {
        cout << "Give the object a name: create {type} ->{name}<-" << endl;
    }
}

void Dialog::listAllStreets()
{
    int index = 0;
    for (auto street : CityParts::streetVector)
    {
        index++;
        cout << index << " [Street] " << street.streetName << " [Attached To] " << street.city << endl;
    }
}

void Dialog::listAllCities()
{
    int index = 0;
    for (auto city : CityParts::cityVector)
    {
        index++;
        cout << index << " [City] " << city.cityName << endl;
    }
}


// Command methods--------------------------------------------------
Dialog::Command::Command(string name)
{
    this->commandName = name;
}

void Dialog::Command::loadHelpMessage(string helpMessage)
{
    this->helpMessage = helpMessage;
}

void Dialog::Command::printHelp()
{
    cout << helpMessage << endl;
}