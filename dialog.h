#pragma once

#include <vector>
#include <string>

using namespace std;

class Dialog
{
public:
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
    static void loadDialog();
    static string nextDialog(int);
    static string Dialog::writeText(int);
    static Dialog::Text root_text("1. Create Street\n2. Create City\nEnter the actions number you want to do: ", true);
};