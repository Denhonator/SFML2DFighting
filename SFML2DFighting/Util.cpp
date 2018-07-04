#include "Util.h"
#include <Windows.h>

Util::Util()
{
	files = { "ground.png", "toad.png" };
	//files.push_back("ground.png"); files.push_back("toad.png");
	for (int i = 0; i < files.size(); i++) {
		if (!tex.loadFromFile(files.at(i)))
			_RPT1(0, "Couldn't load %s", files.at(i));
		texture[files.at(i)] = tex;
	}
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

sf::Texture * Util::getTexture(std::string file)
{
	return &texture[file];
}

