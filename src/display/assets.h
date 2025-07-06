#pragma once

#include "../../include/SFML/Graphics.hpp"

namespace Assets
{
	class ObjectTexture
	{
	public:
		std::string name;
		sf::Texture* texture;

		ObjectTexture(const std::string& name, sf::Texture* texture);

		~ObjectTexture();
	};

	void loadDirectoryElements();
	std::vector<std::string> getDirectoryContents(const std::string& path);
	sf::Texture* getObjectTexture(const std::string& name);
	void storeTextureSizes();

	extern std::string path;
	extern float shapeScale;
	extern std::vector<ObjectTexture*> textureVector;
	extern std::string streetTextureName;
	extern std::string pointTextureName;
	extern std::string routeMarkTextureName;
	extern float pointSize;
	extern float streetSizeX;
	extern float streetSizeY;
}
