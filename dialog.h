#pragma once

#include <fstream>
#include "city.h"

class Dialog
{
public:
    const string HELP_COMMAND = "help";
    const string CREATE_COMMAND = "create";
    const string EDIT_COMMAND = "edit";
    const string LIST_COMMAND = "list";

    const string PATH_TO_HELP = "commandsHelp//help.txt";
   
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

    //list handle-----------------------------------------------
    static void handleListObject(vector<string>);

    //edit handle-----------------------------------------------
    static void editStreet(int index);
    static void editCity(int index);
    static void listEditActions();

    //command handle--------------------------------------------
    static void processInput(vector<string>);
    static void handleHelp(string);
    static void handleCommands(vector<string>);
    static void handleCreateObjects(vector<string>);
    static void handleEditObjects(vector<string>);
    static void errorHandle(string);
};
