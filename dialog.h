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

        Command(string name);
        void loadCommandHelpMessage(const string& message);
        void printHelp();
    };

    //functions-------------------------------------------------
    static void initializeCommands();
    static string loadHelpFromFile(const string& filePath);
    static void listAllStreets();
    static void listAllCities();
    static vector<string> takeInput(const string& prompt = "none");
    static void invalidInputMessage();

    //list handle-----------------------------------------------
    static void handleListObject(const vector<string>& type);

    //edit handle-----------------------------------------------
    static void handleEditObjects(const vector<string>& properties);
    static void editStreet(int streetIndex);
    static void editCity(int cityIndex);
    static void listEditActions();

    //command handle--------------------------------------------
    static void processInput(vector<string>& userInput);
    static void handleHelp(const string& command);
    static void handleCommands(vector<string>& commandInput);
    static void handleCreateObjects(vector<string>& properties);
    static void errorHandle(const string& commandName);
};

#endif