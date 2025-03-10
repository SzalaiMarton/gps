#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

vector<int> generating()
{
    vector<int> reValue;
    for (int index = 0; index < 5; index++)
    {
        int* value = new int(index);
        reValue.push_back(*value);
        delete value;
    }
    return reValue;
}

int main()
{
    vector<int> temp = generating();
    for (int el : temp)
    {
        cout << el << endl;
    }
    return 0;
}