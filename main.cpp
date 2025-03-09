#include "dialog.h"

int main()
{
    vector<string> commands;
    Dialog::initializeCommands();
    do
    {
        commands = Dialog::takeInput();
        if (commands[0] != "exit")
        {
            Dialog::processInput(commands);
        }
        commands.clear();
    } while (commands[0] != "exit");
    return 0;
}