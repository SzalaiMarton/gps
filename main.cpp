#include "dialog.h"
#include <typeinfo>
#include <sstream>

int main()
{
    string input;
    vector<string> command;
    Dialog::initializeCommands();
    do
    {
        //processing input from user
        getline(cin, input);
        if (input != "exit")
        {
            stringstream temp(input);
            string token;
            while (getline(temp, token, ' '))
            {
                command.push_back(token);
            }
            Dialog::processInput(command);
        }
        command.clear();
    } while (input != "exit");
    return 0;
}