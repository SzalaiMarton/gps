#include "dialog.h"
#include "city.h"

string Dialog::writeText(Dialog::Text* outputText, int currentDialogIndex = 100)
{
    string response = "";
    if (!outputText->text.empty())
    {
        cout << outputText->text << endl; 
    }
    
    if (outputText->needInput)
    {
        cin >> response;
        return response;
    }
    else if (outputText->listItems)
    {
        printListElementsToConsole(currentDialogIndex);
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

void Dialog::loadDialog(Dialog::Text& dialog, Dialog::Text& rootDialog, string path)
{
    string tempText;
    Dialog::Text* pPreviousDialogAddress = nullptr;
    Dialog::Text* pCurrentDialog = &dialog;
    
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << path << endl;
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
        else if (tempText == "l")
        {
            if (pPreviousDialogAddress) {
                pPreviousDialogAddress->listItems = true;
            }
            continue;
        }
        pCurrentDialog->text = tempText;
        Dialog::Text* nextDialog = new Dialog::Text("", false);
        pCurrentDialog->nextTexts.push_back(nextDialog);
        pPreviousDialogAddress = pCurrentDialog;

        pCurrentDialog = nextDialog;
    }

    if(pCurrentDialog->text.empty())
    {
        pPreviousDialogAddress->nextTexts.pop_back();
    }

    rootDialog.nextTexts.push_back(&dialog);
    file.close();
}

Dialog::Text* Dialog::getNextText(Dialog::Text* currentDialog, int index)
{
    Dialog::Text wrongInput("wrong", false, false);
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

int Dialog::processInput(string response)
{
    if (response == "1")
    {
        if (CityParts::cityVector.empty())
        {
            return Dialog::CITY_ERROR_MESSAGE;
        }
        else
        {
            return Dialog::CREATE_STREET_INDEX;
        }
    }
    else if (response == "2")
    {
        return Dialog::CREATE_CITY_INDEX;
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

void Dialog::printListElementsToConsole(int currentDialogIndex)
{
    switch (currentDialogIndex)
    {
    case Dialog::CREATE_STREET_INDEX:
        Dialog::printStreets();
        break;
    case Dialog::CREATE_CITY_INDEX:
        Dialog::printCities();
        break;
    default:
        cout << "Cannot list without index" << endl;
        return;
    }
}

void Dialog::printStreets()
{
    for (auto el : CityParts::streetVector)
    {
        cout << el.street_name << endl;
    }
}

void Dialog::printCities()
{
    for (auto el : CityParts::cityVector)
    {
        cout << el.cityName << endl;
    }
}

void Dialog::createObject(Dialog::Text* currentDialog, int currentDialogIndex)
{
    vector<string> response;
    for (auto item : currentDialog->nextTexts)
    {
        response.push_back(Dialog::writeText(item, currentDialogIndex));
    }
    if(currentDialogIndex == Dialog::CREATE_STREET_INDEX)
    {
        CityParts::createStreet(response);
    }
    else if(currentDialogIndex == Dialog::CREATE_CITY_INDEX)
    {
        CityParts::createCity(response);
    }
}