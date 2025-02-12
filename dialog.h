#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "city.h"

using namespace std;

class Dialog
{
public:
    const static int CREATE_STREET_INDEX = 0;
    const static int CREATE_CITY_INDEX = 1;
    const static int CITY_ERROR_MESSAGE = 2;

    struct Text
    {
        vector<Text*> nextTexts;
        string text;
        bool needInput;
        bool listItems;
        Text(string input_text, bool input, bool list = false)
        {
            text = input_text;
            needInput = input;
            listItems = list;
        }
    };

    //functions-------------------------------------------------
    string writeText(Dialog::Text*, int);
    Dialog::Text* getNextText(Dialog::Text*, int);
    bool checkIfDialogIsValid(Dialog::Text*);
    int processInput(string response, vector<CityParts::City>&);
    void printListElementsToConsole(int);
    void Dialog::printStreets();
    void Dialog::printCities();

    //createing objects-----------------------------------------
    void createObjectDialog(Dialog::Text*, int);


    //load dialogs----------------------------------------------
    void loadRootText(Dialog::Text&);
    void loadDialog(Dialog::Text&, Dialog::Text&, string);

    Dialog::Text* pRootDialogAdress;
};