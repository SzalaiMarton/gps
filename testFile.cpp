#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;


vector<string> takeInput()
{
    string input;
    vector<string> command;
    cout << ":";
    getline(cin, input);
    stringstream temp(input);
    string token;
    while (getline(temp, token, ' '))
    {
        cout << token << endl;
        command.push_back(token);
    }
    return command;
}

int main()
{
    takeInput();
}