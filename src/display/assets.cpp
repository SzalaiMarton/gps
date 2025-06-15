#include "assets.h"

namespace Assets
{
    std::string path = "res\\";
    float shapeScale = 1.f;
    std::vector<ObjectTexture*> textureVector = {};
    std::string streetTextureName = "s_street_marked";
    std::string rootTextureName = "s_street_marked";
    std::string pointTextureName = "marked_point";
}

Assets::ObjectTexture::ObjectTexture(const std::string& name, sf::Texture* texture)
{
	this->name = name;
	this->texture = texture;
	this->texture = texture;
}

void Assets::loadDirectoryElements()
{
    std::vector<std::string> contents = Assets::getDirectoryContents(Assets::path);
    for (auto& text : contents)
    { 
        sf::Texture* temp = new sf::Texture();
        std::string tex = Assets::path + text;
        temp->loadFromFile(tex);
        Assets::textureVector.emplace_back(new Assets::ObjectTexture(text, temp));
    }
}

std::vector<std::string> Assets::getDirectoryContents(const std::string& path) 
{
    std::vector<std::string> contents;
    try
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            contents.emplace_back(entry.path().filename().string());
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
    std::string nameWithPng = name + ".png";
    for (auto& text : Assets::textureVector)
    {
        if (text->name == nameWithPng)
        {
            return text->texture;
        }
    }
    return nullptr;
}