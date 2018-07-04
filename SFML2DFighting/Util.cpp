#include "Util.h"

uint32_t Util::frame = 0;
int Util::refresh = 30;
bool Util::initd = false;
sf::Font Util::font;
sf::Text Util::text;
sf::Clock Util::clock[3];
sf::String Util::string = "";
std::string Util::stdring = "";

Util::Util()
{
	files = { "ground.png", "toad.png" };
	for (int i = 0; i < files.size(); i++) {
		if (!tex.loadFromFile(files.at(i)))
			_RPT1(0, "Couldn't load %s", files.at(i));
		texture[files.at(i)] = tex;
	}
}

sf::Text& Util::log()
{
	if (!initd) {
		font.loadFromFile("arial.ttf");
		text.setFont(font);
		text.setCharacterSize(14);
		initd = true;
	}
	if (frame % refresh == 0 && string != "") {
		text.setString(string);
		string = "";
	}
	frame++;
	return text;
}

Util::~Util()
{
}

bool Util::rectOverlap(sf::FloatRect rect, sf::FloatRect rect2)
{
	if ((rect2.left > rect.left&&rect2.left < rect.left + rect.width
		|| rect2.left<rect.left&&rect2.left + rect2.width>rect.left)
		&& (rect2.top > rect.top&&rect2.top < rect.top + rect.height
			|| rect2.top<rect.top&&rect2.top + rect2.height>rect.top))
		return true;
	return false;
}

void Util::add(sf::String str)
{
	if (frame % refresh == 0 && string.find(str) == sf::String::InvalidPos)
		string += str + "\n";
}

void Util::addTime(sf::String str, int inst)
{
	if(frame%refresh==0 && string.find(str)==sf::String::InvalidPos)
		string += str + ": " + std::to_string(clock[inst].getElapsedTime().asMicroseconds())+"\n";
	clock[inst].restart();
}

void Util::resetClock(int inst)
{
	clock[inst].restart();
}

sf::Texture * Util::getTexture(std::string file)
{
	return &texture[file];
}

