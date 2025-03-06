#include "dialog.h"
#include <typeinfo>

int main()
{
    string input = "";
    Dialog::initializeCommands();
    do
    {
        cout << "write" << endl;
        cin >> input;
        //Dialog::processInput(input);
    } while (input != "exit");
    return 0;
}