#include "Debug.h"

sf::Text& Debug::log()
{
	if (frame % refresh == 0 && str!="") {
		text.setString(str);
		str = "";
	}
	frame++;
	return text;
}

void Debug::add(sf::String string)
{
	if (frame % refresh == 0 && string != "")
		str += string+"\n";
}

Debug::Debug()
{
	font.loadFromFile("arial.ttf");
	text.setFont(font);
	text.setCharacterSize(14);
	frame = 0;
	str = "";
}


Debug::~Debug()
{
}
