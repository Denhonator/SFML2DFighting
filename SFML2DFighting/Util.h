#pragma once
#include "SFML\Graphics.hpp"
#include <unordered_map>
class Util
{
public:
	Util();
	~Util();
	static bool rectOverlap(sf::FloatRect rect, sf::FloatRect rect2);
	sf::Texture* getTexture(std::string file);
private:
	std::unordered_map<std::string, sf::Texture> texture;
	sf::Texture tex;
	std::vector<std::string> files;
};

