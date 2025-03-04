#include "dialog.h"
#include "city.h"

Dialog::Command MainHelp("help", "help");
Dialog::Command Create("create", "create -h");
Dialog::Command Edit("edit", "edit -h");

void Dialog::initializeCommands()
{
    Dialog dialog;
    MainHelp.loadHelpMessage(loadHelpFromFile(dialog.pathToHelp));
    Create.loadHelpMessage(loadHelpFromFile(dialog.pathToCreate));
    Edit.loadHelpMessage(loadHelpFromFile(dialog.pathToEdit));
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

void Dialog::processInput(string input)
{
    //handling help commands
    if(input.find("-h") != string::npos)
    {
        handleHelp(input);
    }
    else
    {
        handleCommands(input);
    }
}

void Dialog::handleHelp(string input)
{
    if(input == MainHelp.commandName) {MainHelp.printHelp();}
    else if(input == Create.helpCommand) {Create.printHelp();}
    else if(input == Edit.helpCommand) {Edit.printHelp();}
}

void Dialog::handleCommands(string input)
{

}