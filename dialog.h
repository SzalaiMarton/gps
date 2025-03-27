#ifndef DIALOG_H
#define DIALOG_H

#include <fstream>
#include "city.h"

class Dialog
{
public:
    const string PATH_TO_HELP = "commandsHelp//help.txt";
   
    class Command
    {
    private:
        string helpMessage;
    public:
        string commandName;

        Command(string);
        void loadCommandHelpMessage(const string&);
        void printHelp();
    };

    //functions-------------------------------------------------
    static void initializeCommands();
    static string loadHelpFromFile(const string&);
    static void listAllStreets();
    static void listAllCities();
    static vector<string> takeInput(const string& i = "none");
    static void invalidInputMessage();

    //list handle-----------------------------------------------
    static void handleListObject(const vector<string>&);

    //edit handle-----------------------------------------------
    static void handleEditObjects(const vector<string>&);
    static void editStreet(int index);
    static void editCity(int index);
    static void listEditActions();

    //command handle--------------------------------------------
    static void processInput(vector<string>&);
    static void handleHelp(const string&);
    static void handleCommands(vector<string>&);
    static void handleCreateObjects(const vector<string>&);
    static void errorHandle(const string&);
};

#endif