#include "assets.h"
#include "../custom_exception.h"

namespace Assets
{
    std::string path = "res\\";
    int shapeScale = 1.f;
    std::vector<ObjectTexture*> textureVector = {};
}

Assets::ObjectTexture::ObjectTexture(const std::string& name, sf::Texture* texture)
{
	this->name = name;
	this->texture = texture;
}

void Assets::loadDirectoryElements()
{
    std::vector<std::string> contents = Assets::getDirectoryContents(Assets::path);
    for (const auto& text : contents)
    {
        sf::Texture* temp = new sf::Texture();
        if (!temp->loadFromFile(text))
        {
            delete temp;
            throw new CustomExceptions::FileOrFolderCannotBeFoundException("Assets::loadDirectoryElements");
            continue;
        }
        Assets::textureVector.emplace_back(new Assets::ObjectTexture(text.substr(Assets::path.length(), text.find(".") - Assets::path.length()), temp));
    }
}

std::vector<std::string> Assets::getDirectoryContents(const std::string& path) 
{
    std::vector<std::string> contents;
    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            contents.emplace_back(entry.path().string());
        }
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        std::cout << "Error accessing directory: " << e.what() << std::endl;
    }
    return contents;
}

sf::Texture* Assets::getObjectTexture(const std::string& name)
{
    for (auto& text : Assets::textureVector)
    {
        if (text->name == name)
        {
            return text->texture;
        }
    }
    return nullptr;
}