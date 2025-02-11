#include <iostream>
#include "city.h"
#include "dialog.h"

int main()
{
    Dialog::loadDialog();
    string response = Dialog::writeText(firstDialog);
    while(response != "exit")
    {
        
    }
}