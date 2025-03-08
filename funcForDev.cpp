#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Custom
{
public:
    static void printStringVector(vector<string> vec)
    {
        for (string el : vec)
        {
            cout << "[Custom] " << el << endl;
        }
    }

    static void printIntVector(vector<int> vec)
    {
        for (int el : vec)
        {
            cout << "[Custom] " << el << endl;
        }
    }
};