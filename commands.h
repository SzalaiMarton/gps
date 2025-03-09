#pragma once

#include <string>

using namespace std;

class Commands
{
public:
    const string EDIT_CITY_NAME = "name";
    const string EDIT_CITY_POPULATION = "population";
    const string EDIT_CITY_STREETS = "streets";

    const string EDIT_STREET_NAME = "name";
    const string EDIT_STREET_LENGTH = "length";
    const string EDIT_STREET_CITY = "city";
    const string EDIT_STREET_BUILDINGS = "buildings";
    const string EDIT_STREET_STREETS_BACK = "sback";
    const string EDIT_STREET_STREETS_FRONT = "sfront";

    const string EDIT_APPEND_COMMAND = "append";//for vectors
    const string EDIT_DELETE_COMMAND = "delete";//cant use for single value besides street->city
    const string EDIT_CHANGE_COMMAND = "change";//for single value

    const string HELP_COMMAND = "help";
    const string CREATE_COMMAND = "create";
    const string EDIT_COMMAND = "edit";
    const string LIST_COMMAND = "list";
};