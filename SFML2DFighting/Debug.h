#pragma once
#include <SFML\Graphics.hpp>
class Debug
{
public:
	sf::Text& log();
	void add(sf::String string);
	Debug();
	~Debug();
	sf::Text text;
	sf::Font font;
	sf::String str;
	int refresh = 20;
private:
	uint32_t frame;
};

