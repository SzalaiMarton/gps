#include "dialog.h"
#include "city.h"
#include "commands.h"

Commands command;
Dialog dialog;

Dialog::Command MainHelp(command.HELP_COMMAND);
Dialog::Command Create(command.CREATE_COMMAND);
Dialog::Command Edit(command.EDIT_COMMAND);
Dialog::Command List(command.LIST_COMMAND);

void Dialog::initializeCommands()
{
    MainHelp.loadCommandHelpMessage(loadHelpFromFile(dialog.PATH_TO_HELP));
    Create.loadCommandHelpMessage("To create an object type: create {type} {name}. After creation use edit to add properties to it.");
    Edit.loadCommandHelpMessage("To edit an object first select the object: edit {type}.");
    List.loadCommandHelpMessage("To list an object vector type: list {type}.");
}

string Dialog::loadHelpFromFile(const string& path)
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

void Dialog::processInput(vector<string>& input)
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

void Dialog::handleHelp(const string& input)
{
    if(input == MainHelp.commandName) {MainHelp.printHelp();}
    else if(input == Create.commandName) {Create.printHelp();}
    else if(input == Edit.commandName) {Edit.printHelp();}
    else if(input == List.commandName) {List.printHelp();}
}

void Dialog::handleCommands(vector<string>& input)
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
    else
    {
        Dialog::invalidInputMessage();
    }
}

void Dialog::handleCreateObjects(const vector<string>& properties)
{
    if (properties[0] == "street")
    {
        CityParts::createStreet(properties);
    }
    else if (properties[0] == "city")
    {
        CityParts::createCity(properties);
    }
    else
    {
        Dialog::invalidInputMessage();
    }
}

void Dialog::handleEditObjects(const vector<string>& properties)
{
    vector<string> input;
    if (properties[0] == "street")
    {
        if (CityParts::streetVector.empty() || CityParts::streetVector.size() == 0)
        {
            cout << "No streets have been created." << endl;
            return;
        }
        Dialog::listAllStreets();
        do
        {
            input = Dialog::takeInput("Type in the index of the street you want to edit: ");
            if (input.size() != 1)
            {
                input.clear();
            }
            if (stoi(input[0]) - 1 > CityParts::streetVector.size() - 1)
            {
                cout << "Street not found." << endl;
                return;
            }
        } while (stoi(input[0]) - 1 > CityParts::streetVector.size() - 1);
        Dialog::editStreet(stoi(input[0]) - 1);
    }
    else if (properties[0] == "city")
    {
        if (CityParts::cityVector.empty() || CityParts::cityVector.size() == 0)
        {
            cout << "No cities have been created." << endl;
            return;
        }
        Dialog::listAllCities();
        do
        {
            input = Dialog::takeInput("Type in the index of the street you want to edit: ");
            if (stoi(input[0]) - 1 > CityParts::cityVector.size() - 1)
            {
                cout << "City not found." << endl;
                return;
            }
        } while (stoi(input[0]) - 1 > CityParts::cityVector.size() - 1);
        Dialog::editCity(stoi(input[0]) - 1);
    }
    else
    {
        Dialog::invalidInputMessage();
    }
}

void Dialog::editStreet(int index)
{
    CityParts::streetVector[index].listProperties();
    cout << "To edit an element type: {property} {action}" << endl;
    cout << "To list actions type: edit actions -l" << endl;
    cout << "To quit editing type: edit quit" << endl;
    vector<string> input;
    do
    {
        input = Dialog::takeInput();
        if (input[0] == "edit" && input[1] == "actions" && input[2] == "-l")
        {
            Dialog::listEditActions();
            continue;
        }
        if (input[0] == command.EDIT_STREET_NAME)
        {
            if (input[1] == command.EDIT_CHANGE_COMMAND)
            {
                CityParts::streetVector[index].editName();
                return;
            }
            else if (input[1] == command.EDIT_DELETE_COMMAND)
            {
                cout << "Command cannot be used on this property" << endl;
            }
        }
        else if (input[0] == command.EDIT_STREET_CITY)
        {
            if (input[1] == command.EDIT_CHANGE_COMMAND)
            {
                CityParts::streetVector[index].editCity();
                return;
            }
            else if (input[1] == command.EDIT_DELETE_COMMAND)
            {
                CityParts::streetVector[index].deleteCity();
                return;
            }
        }
        else if (input[0] == command.EDIT_STREET_BUILDINGS)
        {
            //CityParts::streetVector[index].editBuildings();
            cout << "not finished yet" << endl;
        }
        else if (input[0] == command.EDIT_STREET_STREETS_BACK)
        {
            //CityParts::streetVector[index].editConnectedStreetsBack();
            cout << "not finished yet" << endl;
        }
        else if (input[0] == command.EDIT_STREET_STREETS_FRONT)
        {
            //CityParts::streetVector[index].editConnectedStreetsFront();
            cout << "not finished yet" << endl;
        }
        input.clear();
    } while (input[0] != "edit" && input[1] != "exit");
}

void Dialog::editCity(int index)
{
    CityParts::cityVector[index].listProperties();
    cout << "To edit an element type: {property} {action}" << endl;
    cout << "To list actions type: edit actions -l" << endl;
    cout << "To quit editing type: edit quit" << endl;
    vector<string> input;
    do
    {
        input = Dialog::takeInput();
        if (input[0] == "edit" && input[1] == "actions" && input[2] == "-l")
        {
            Dialog::listEditActions();
            continue;
        }
        else if (input[0] == command.EDIT_CITY_NAME)
        {
            if (input[1] == command.EDIT_CHANGE_COMMAND)
            {
                CityParts::cityVector[index].editName();
                return;
            }
            else if (input[1] == command.EDIT_DELETE_COMMAND)
            {
                cout << "Command cannot be used on this property" << endl;
            }
        }
        else if (input[0] == command.EDIT_CITY_STREETS)
        {
            //CityParts::cityVector[index].editStreets();
            cout << "not finished yet" << endl;
        }
        input.clear();
    } while (input[0] != "edit" && input[1] != "exit");
}

void Dialog::listEditActions()
{
    cout << command.EDIT_DELETE_COMMAND << "      " << "deletes the property's value (cannot use on single value beside street->city)" << endl;
    cout << command.EDIT_CHANGE_COMMAND << "      " << "changes the property's value (don't work on vectors)" << endl;
    cout << command.EDIT_APPEND_COMMAND << "      " << "append an element to vectors (don't work on single value properties)" << endl;
}

void Dialog::errorHandle(const string& commandName)
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

vector<string> Dialog::takeInput(const string& customText)
{
    string input;
    vector<string> command;
    if (customText == "none")
    {
        cout << ": ";
    }
    else
    {
        cout << customText;
    }
    getline(cin, input);
    stringstream temp(input);
    string token;
    while (getline(temp, token, ' '))
    {
        command.emplace_back(token);
    }
    return command;
}

void Dialog::invalidInputMessage()
{
    cout << "Unknown command." << endl;
    return;
}

void Dialog::handleListObject(const vector<string>& type)
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
    else
    {
        Dialog::invalidInputMessage();
    }
}


// Command methods--------------------------------------------------
Dialog::Command::Command(string name)
{
    this->commandName = name;
}

void Dialog::Command::loadCommandHelpMessage(const string& helpMessage)
{
    this->helpMessage = helpMessage;
}

void Dialog::Command::printHelp()
{
    cout << helpMessage << endl;
}