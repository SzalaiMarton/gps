#include "dialog.h"
#include "city.h"

string Dialog::writeText(Dialog::Text* outputText)
{
    string response = "";
    cout << outputText->text << endl;
    if (outputText->needInput)
    {
        cin >> response;
        return response;
    }
    return response;
}

void Dialog::loadRootText(Dialog::Text& dialog)
{
    string tempText;
    ifstream file("dialogs//rootDialog.txt");
    while(getline(file, tempText))
    {
        if (tempText == "i")
        {
            dialog.needInput = true;
            file.close();
            return;
        }
        else
        {
            dialog.text += tempText + "\n";
        }
    }
    file.close();
}

void Dialog::loadCreateStreetText(Dialog::Text& dialog, Dialog::Text& rootDialog)
{
    string tempText;
    Dialog::Text* pPreviousDialogAddress = nullptr;
    Dialog::Text* pCurrentDialog = &dialog;
    
    ifstream file("dialogs//createStreet.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open file dialogs//createStreet.txt" << endl;
        return;
    }

    while (getline(file, tempText))
    {
        if (tempText == "i")
        {
            if (pPreviousDialogAddress) {
                pPreviousDialogAddress->needInput = true;
            }
            continue;
        }

        pCurrentDialog->text = tempText;
        Dialog::Text* nextDialog = new Dialog::Text("", false);
        pCurrentDialog->nextTexts.push_back(nextDialog);
        pPreviousDialogAddress = pCurrentDialog;

        pCurrentDialog = nextDialog;
    }

    rootDialog.nextTexts.push_back(&dialog);
    file.close();
}

void Dialog::loadCreateStreetText(Dialog::Text& dialog, Dialog::Text& rootDialog)
{
    string tempText;
    Dialog::Text* pPreviousDialogAddress = nullptr;
    Dialog::Text* pCurrentDialog = &dialog;
    
    ifstream file("dialogs//createCity.txt");
    if (!file.is_open()) {
        cerr << "Error: Could not open file dialogs//createCity.txt" << endl;
        return;
    }

    while (getline(file, tempText))
    {
        if (tempText == "i")
        {
            if (pPreviousDialogAddress) {
                pPreviousDialogAddress->needInput = true;
            }
            continue;
        }
        pCurrentDialog->text = tempText;
        Dialog::Text* nextDialog = new Dialog::Text("", false);
        pCurrentDialog->nextTexts.push_back(nextDialog);
        pPreviousDialogAddress = pCurrentDialog;

        pCurrentDialog = nextDialog;
    }
    rootDialog.nextTexts.push_back(&dialog);
    file.close();
}

void Dialog::loadCityErrorText(Dialog::Text& dialog, Dialog::Text& rootDialog)
{
    string tempText;
    ifstream file("dialogs//cityErrorMessage.txt");
    rootDialog.nextTexts.push_back(&dialog);
    while(getline(file, tempText))
    {
        dialog.text = tempText;
    }
    file.close();
}

Dialog::Text* Dialog::getNextText(Dialog::Text* currentDialog, int index)
{
    Dialog::Text wrongInput("wrong", false);
    Dialog::Text* pWrongInputAddress = &wrongInput;
    if (currentDialog->nextTexts.size() == 0)
    {
        return pWrongInputAddress;
    }
    else if (currentDialog->nextTexts.size() < index + 1)
    {
        return pWrongInputAddress;
    }
    return currentDialog->nextTexts[index];
}

bool Dialog::checkIfDialogIsValid(Dialog::Text* currentDialog)
{
    if (currentDialog->text == "wrong")
    {
        return false;
    }
    return true;
}

int Dialog::processInput(string response, vector<CityParts::City>& cityVector)
{
    if (response == "1")
    {
        return Dialog::CREATE_STREET_INDEX;
    }
    else if (response == "2")
    {
        if (cityVector.empty())
        {
            return Dialog::CITY_ERROR_MESSAGE;
        }
        else
        {
            return Dialog::CREATE_CITY_INDEX;
        }
    }
    else if (response == "exit")
    {
        return -1;
    }
    else
    {
        return 100;
    }
}