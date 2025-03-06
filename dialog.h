#pragma once

#include <fstream>
#include "city.h"

class Dialog
{
public:
    const string helpCommand = "help";
    const string createCommand = "create";
    const string editCommand = "edit";

    const string pathToHelp = "commandsHelp//help.txt";
    const string pathToCreate = "commandsHelp//create.txt";
    const string pathToEdit = "commandsHelp//edit.txt";
   
    class Command
    {
    private:
        string helpMessage;
    public:
        string commandName;
        string helpCommand;

        Command(string, string);
        void loadHelpMessage(string);
        void printHelp();
    };

    //functions-------------------------------------------------
    static void initializeCommands();
    static string loadHelpFromFile(string);
    static void processInput(string);
    static void handleHelp(string);
    static void handleCommands(string);
    static vector<string> sliceCommand(string, Command); //[0] - type, [1] - name
};