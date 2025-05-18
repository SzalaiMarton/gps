#pragma once

#include "../../include/SFML/Graphics.hpp"
#include <filesystem>
#include <iostream>

namespace Assets
{
	class ObjectTexture
	{
	public:
		std::string name;
		sf::Texture* texture;

		ObjectTexture(const std::string& name, sf::Texture* texture);
	};

	

	void loadDirectoryElements();
	std::vector<std::string> getDirectoryContents(const std::string& path);
	sf::Texture* getObjectTexture(const std::string& name);

	extern std::string path;
	extern int shapeScale;
	extern std::vector<ObjectTexture*> textureVector;
}
