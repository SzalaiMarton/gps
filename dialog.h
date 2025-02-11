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
    const int CREATE_STREET_INDEX = 0;
    const int CREATE_CITY_INDEX = 1;
    const int CITY_ERROR_MESSAGE = 2;

    struct Text
    {
        vector<Text*> nextTexts;
        string text;
        bool needInput;
        Text(string input_text, bool input)
        {
            text = input_text;
            needInput = input;
        }
    };

    //functions-------------------------------------------------
    string writeText(Dialog::Text*);
    Dialog::Text* getNextText(Dialog::Text*, int);
    bool checkIfDialogIsValid(Dialog::Text*);
    int processInput(string response, vector<CityParts::City>&);

    //load dialogs----------------------------------------------
    void loadRootText(Dialog::Text&);
    void loadCreateStreetText(Dialog::Text&, Dialog::Text&);
    void loadCreateCityText(Dialog::Text&, Dialog::Text&);
    void loadCityErrorText(Dialog::Text&, Dialog::Text&);


    Dialog::Text* pRootDialogAdress;
};