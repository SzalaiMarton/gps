#pragma once

#include <fstream>
#include "city.h"

class Dialog
{
public:
    const string HELP_COMMAND = "help";
    const string CREATE_COMMAND = "create";
    const string EDIT_COMMAND = "edit";

    const string PATH_TO_HELP = "commandsHelp//help.txt";
    const string PATH_TO_CREATE = "commandsHelp//create.txt";
    const string PATH_TO_EDIT = "commandsHelp//edit.txt";
   
    class Command
    {
    private:
        string helpMessage;
    public:
        string commandName;

        Command(string);
        void loadHelpMessage(string);
        void printHelp();
    };

    //functions-------------------------------------------------
    static void initializeCommands();
    static string loadHelpFromFile(string);
    static void listAllStreets();
    static void listAllCities();

    //command handle---------------------------------------------
    static void processInput(vector<string>);
    static void handleHelp(string);
    static void handleCommands(vector<string>);
    static void handleCreateObjects(vector<string>);
    static void handleEditObjects(vector<string>);
    static void errorHandle(string);
};
