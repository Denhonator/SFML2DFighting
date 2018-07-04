#pragma once
#include "SFML\Graphics.hpp"
#include <unordered_map>
#include <Windows.h>
class Util
{
public:
	Util();
	static sf::Text & log();
	~Util();
	static bool rectOverlap(sf::FloatRect rect, sf::FloatRect rect2);
	static void add(sf::String str);
	static void addTime(sf::String str, int inst = 0);
	static void resetClock(int inst = 0);
	sf::Texture* getTexture(std::string file);
	static int refresh;
private:
	std::unordered_map<std::string, sf::Texture> texture;
	sf::Texture tex;
	std::vector<std::string> files;
	static sf::Text text;
	static sf::Font font;
	static sf::Clock clock[];
	static sf::String string;
	static std::string stdring;
	static uint32_t frame;
	static bool initd;
};