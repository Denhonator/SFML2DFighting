#include "Hitbox.h"

Hitbox::Hitbox(sf::Vector2f boxSize, sf::Vector2f position)
{
	size = boxSize;
	rectangle = sf::RectangleShape(boxSize);
	pos = position;
	rectangle.setFillColor(sf::Color(255, 0, 0, 128));
	solid = true;
	rectangle.setPosition(pos);
}

Hitbox::Hitbox() {
	size = sf::Vector2f(0, 0);
	rectangle = sf::RectangleShape(size);
	pos = sf::Vector2f(0, 0);
	rectangle.setFillColor(sf::Color(0, 0, 0));
	solid = true;
}

Hitbox::~Hitbox()
{
}

sf::RectangleShape Hitbox::getRectangle()
{
	return rectangle;
}

void Hitbox::update()
{
	if (solid) {
		rectangle.setPosition(pos);
		rectangle.setSize(size);
	}
}