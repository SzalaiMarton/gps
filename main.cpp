#include "city.h"
#include "dialog.h"
#include <typeinfo>

Dialog dialog;
string response = "first";
Dialog::Text* currentDialog;
int currentDialogIndex;

string createStreetPath = "dialogs//createStreet.txt";
string createCityPath = "dialogs//createCity.txt";
string cityErrorMessageDialog = "dialogs//cityErrorMessage.txt";

int main()
{
    Dialog::Text rootDialog("", false);
    dialog.loadRootText(rootDialog);
    dialog.pRootDialogAdress = &rootDialog;

    Dialog::Text createStreetDialog("", false);
    dialog.loadDialog(createStreetDialog, rootDialog, createStreetPath);

    Dialog::Text createCityDialog("", false);
    dialog.loadDialog(createCityDialog, rootDialog, createCityPath);

    Dialog::Text cityErrorMessage("", false);
    dialog.loadDialog(cityErrorMessage, rootDialog, cityErrorMessageDialog);

    while (response != "exit")
    {
        currentDialog = dialog.pRootDialogAdress;
        response = dialog.writeText(currentDialog, 100);      
        currentDialogIndex = dialog.processInput(response);

        if (currentDialogIndex == -1)
        {
            return 0;
        }
        
        currentDialog = dialog.getNextText(currentDialog, currentDialogIndex);

        if (!dialog.checkIfDialogIsValid(currentDialog))
        {
            cout << "Wrong input." << endl;
            currentDialog = dialog.pRootDialogAdress;
        }
        else
        {
            dialog.createObjectDialog(currentDialog, currentDialogIndex);
        }
    }
    return 0;
}