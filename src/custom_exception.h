#pragma once

#include <string>

namespace CustomExceptions
{
    class CityExceptions : public std::exception
    {
    public:
        CityExceptions(const std::string& message) : msg_(message) {}
        const char* what() const noexcept override { return msg_.c_str(); }
    private:
        const std::string msg_;
    };

    class NullInputException : public CityExceptions
    {
    public:
        NullInputException(const std::string& funcName) : CityExceptions("Null pointer has been given to this function: " + funcName) {}
    };

    class VectorEmptyException : public CityExceptions
    {
    public:
        VectorEmptyException(const std::string& funcName) : CityExceptions("Vector is empty, in this function: " + funcName) {}
    };

    class FileOrFolderCannotBeFoundException : public CityExceptions
    {
    public:
        FileOrFolderCannotBeFoundException(const std::string& funcName) : CityExceptions("File or folder cannot be found, from this function: " + funcName) {}
    };
}
