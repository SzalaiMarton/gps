#include "city.h"
#include "dialog.h"
#include <typeinfo>

Dialog dialog;
string response = "first";
Dialog::Text* currentDialog;
int nextDialogIndex;

CityParts cityparts;

int main()
{
    vector<CityParts::City> cityVector;

    Dialog::Text rootDialog("", false);
    dialog.loadRootText(rootDialog);
    dialog.pRootDialogAdress = &rootDialog;

    Dialog::Text createStreetDialog("", false);
    dialog.loadCreateStreetText(createStreetDialog, rootDialog);

    Dialog::Text createCityDialog("", false);
    dialog.loadCreateCityText(createCityDialog, rootDialog);

    Dialog::Text cityErrorMessage("", false);
    dialog.loadCityErrorText(cityErrorMessage, rootDialog);

    currentDialog = dialog.pRootDialogAdress;

    while (response != "exit")
    {
        response = dialog.writeText(currentDialog);      
        nextDialogIndex = dialog.processInput(response, cityVector);

        if (nextDialogIndex == -1)
        {
            return 0;
        }
        
        currentDialog = dialog.getNextText(currentDialog, nextDialogIndex);

        if (!dialog.checkIfDialogIsValid(currentDialog))
        {
            cout << "Wrong input." << endl;
            currentDialog = dialog.pRootDialogAdress;
        }
    }
    return 0;
}