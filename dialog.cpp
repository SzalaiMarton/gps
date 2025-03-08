#include "dialog.h"
#include "city.h"

Dialog dialog;

Dialog::Command MainHelp(dialog.HELP_COMMAND);
Dialog::Command Create(dialog.CREATE_COMMAND);
Dialog::Command Edit(dialog.EDIT_COMMAND);
Dialog::Command List(dialog.LIST_COMMAND);

void Dialog::initializeCommands()
{
    MainHelp.loadHelpMessage(loadHelpFromFile(dialog.PATH_TO_HELP));
    Create.loadHelpMessage("To create an object type: create {type} {name}. After creation use edit to add properties to it.");
    Edit.loadHelpMessage("To edit an object first select the object: edit {type}.");
    List.loadHelpMessage("To list an object vector type: list {type}.");
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
    for (string el : input)
    {
        if(el == "-h" || el == "help")
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
    else if(input == List.commandName) {List.printHelp();}
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
    else if (input[0] == List.commandName)
    {
        input.erase(input.begin());
        if (input.size() == 0)
        {
            Dialog::errorHandle(List.commandName);
            return;
        }
        Dialog::handleListObject(input);
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
    int input;
    if (properties[0] == "street")
    {
        if (CityParts::streetVector.empty() || CityParts::streetVector.size() == 0)
        {
            cout << "No streets have been created." << endl;
            return;
        }
        Dialog::listAllStreets();
        cout << "Type in the index of the street you want to edit: ";
        do
        {
            cin >> input;
            input--;
            if (input > CityParts::streetVector.size() - 1)
            {
                cout << "Street not found." << endl;
                return;
            }
        } while (input > CityParts::streetVector.size() - 1);
        Dialog::editStreet(input);
    }
    else if (properties[0] == "city")
    {
        if (CityParts::cityVector.empty() || CityParts::cityVector.size() == 0)
        {
            cout << "No cities have been created." << endl;
            return;
        }
        Dialog::listAllCities();
        cout << "Type in the index of the city you want to edit: ";
        do
        {
            cin >> input;
            input--;
            if (input > CityParts::cityVector.size() - 1)
            {
                cout << "City not found." << endl;
                return;
            }
        } while (input > CityParts::cityVector.size() - 1);
        Dialog::editCity(input);
    }
}

void Dialog::editStreet(int index)
{
    CityParts::streetVector[index].listProperties();
    cout << "To edit an element type: {property} {action}" << endl;
    cout << "To list actions type: edit actions -l" << endl;
    vector<string> input = Dialog::takeInput();
}

void Dialog::editCity(int index)
{
    CityParts::cityVector[index].listProperties();
    cout << "To edit an element type: {property} {action}" << endl;
    cout << "To list actions type: edit actions -l" << endl;
    vector<string> input = Dialog::takeInput();
}

void Dialog::listEditActions()
{
    cout << "delete " << "      " << "deletes the property's value" << endl;
    cout << "edit   " << "      " << "changes the property's value (don't work on vectors)" << endl;
    cout << "append " << "      " << "append an element to vectors (don't work on single value properties)" << endl;
}

void Dialog::errorHandle(string commandName)
{
    if (commandName == Create.commandName)
    {
        cout << "Specify the object with type and name: create ->{type}<- ->{name}<-" << endl;
    }
    else if (commandName == Edit.commandName)
    {
        cout << "Specify the object you want to edit: edit ->{type}<-" << endl;
    }
    else if (commandName == List.commandName)
    {
        cout << "Specify the object vector you want to list: list ->{type}<-" << endl;
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

vector<string> Dialog::takeInput()
{
    string input;
    vector<string> command;
    cout << ":";
    getline(cin, input);
    stringstream temp(input);
    string token;
    while (getline(temp, token, ' '))
    {
        command.push_back(token);
    }
    return command;
}

void Dialog::handleListObject(vector<string> type)
{
    if (type[0] == "street")
    {
        if (CityParts::streetVector.empty() || CityParts::streetVector.size() == 0)
        {
            cout << "No streets have been created." << endl;
        }
        else
        {
            Dialog::listAllStreets();
        }
    }
    else if (type[0] == "city")
    {
        if (CityParts::cityVector.empty() || CityParts::cityVector.size() == 0)
        {
            cout << "No cities have been created." << endl;
        }
        else
        {
            Dialog::listAllCities();
        }
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