#include "Hitbox.h"
#include "Scene.h"

Hitbox::Hitbox(sf::Vector2f boxSize, sf::Vector2f position)
{
	size = boxSize;
	rectangle = sf::RectangleShape(boxSize);
	pos = position;
	rectangle.setFillColor(sf::Color(255, 0, 0, 128));
	solid = true;
	rectangle.setPosition(pos);
	speed = sf::Vector2f(0, 0);
}

Hitbox::Hitbox() {
	size = sf::Vector2f(0, 0);
	rectangle = sf::RectangleShape(size);
	pos = sf::Vector2f(0, 0);
	rectangle.setFillColor(sf::Color(0, 0, 0));
	solid = true;
	speed = sf::Vector2f(0, 0);
}

Hitbox::~Hitbox()
{
}

sf::RectangleShape Hitbox::getRectangle()
{
	return rectangle;
}

std::vector<int> Hitbox::hitcheck()
{
	std::vector<int> hits;
	sf::Vector2f fsize;
	sf::Vector2f fpos;
	extern Scene scene;
	for (int i = 0; i<scene.fighter.size(); i++) {
		fsize = scene.fighter.at(i).size;
		fpos = scene.fighter.at(i).pos;
		if (pos.x >= fpos.x&&pos.x <= fpos.x + fsize.x || 
			fpos.x >= pos.x&&fpos.x <= pos.x+size.x) {
			if (pos.y >= fpos.y&&pos.y <= fpos.y + fsize.y ||
				fpos.y >= pos.y&&fpos.y <= pos.y + size.y) {
				hits.push_back(i);
			}
		}
	}
	return hits;
}

void Hitbox::update()
{
	if (solid) {
		rectangle.setPosition(pos);
		rectangle.setSize(size);
	}
}