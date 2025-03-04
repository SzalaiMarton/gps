#pragma once

#include <fstream>
#include "city.h"

class Dialog
{
public:
    const string pathToHelp = "commandsHelp//help.txt";
    const string pathToCreate = "commandsHelp//create.txt";
    const string pathToEdit = "commandsHelp//edit.txt";

    class Command
    {
    private:
        string help;
    public:
        string commandName;
        string helpCommand;

        Command(string name, string helpCom)
        {
            this->commandName = name;
            this->helpCommand = helpCom;
        }

        void loadHelpMessage(string helpMessage)
        {
            this->help = helpMessage;
        }

        void printHelp()
        {
            cout << help << endl;
        }

        vector<string> sliceCommand(string input)
        {
            
        }
    };

    //functions-------------------------------------------------
    void static initializeCommands();
    string static loadHelpFromFile(string path);
    void static processInput(string input);
    void static handleHelp(string input);
    void static handleCommands(string input);
};