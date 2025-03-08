#include "dialog.h"

int main()
{
    vector<string> command;
    Dialog::initializeCommands();
    do
    {
        Dialog::takeInput();
        if (command[0] != "exit")
        {
            Dialog::processInput(command);
        }
        command.clear();
    } while (command[0] != "exit");
    return 0;
}