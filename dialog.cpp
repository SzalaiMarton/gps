#include "dialog.h"
#include "city.h"

Dialog dialog;

Dialog::Command MainHelp(dialog.helpCommand,dialog.helpCommand);
Dialog::Command Create(dialog.createCommand, dialog.createCommand + " -h");
Dialog::Command Edit(dialog.editCommand, dialog.editCommand + " -h");

void Dialog::initializeCommands()
{
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


// Command methods--------------------------------------------------
Dialog::Command::Command(string name, string helpCom)
{
    this->commandName = name;
    this->helpCommand = helpCom;
}

void Dialog::Command::loadHelpMessage(string helpMessage)
{
    this->helpMessage = helpMessage;
}

void Dialog::Command::printHelp()
{
    cout << helpMessage << endl;
}

vector<string> Dialog::sliceCommand(string input, Command command) //[0] - type, [1] - name
{
    cout << "first input: " << input << endl;
    /*int commandInInput = input.find(command.commandName) + command.commandName.length();
    input = input.substr(commandInInput);*/
    cout << "return string: " << input << endl;
    return {input};
}