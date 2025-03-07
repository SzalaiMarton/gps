#include "dialog.h"
#include <typeinfo>
#include <sstream> // Include stringstream for tokenizing

int main()
{
    string input;
    vector<string> inputCommand;
    Dialog::initializeCommands();
    do
    {
        //processing input from user
        getline(cin, input);
        stringstream temp(input);
        string token;
        while (getline(temp, token, ' '))
        {
            inputCommand.push_back(token);
        }

        
    } while (input != "exit");
    return 0;
}