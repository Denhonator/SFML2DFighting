#include "Wall.h"

Wall::Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Color color)
{
	size = boxSize;
	rectangle = sf::RectangleShape(boxSize);
	pos = position;
	rectangle.setFillColor(color);
	solid = true;
	rectangle.setPosition(pos);
}

Wall::Wall() {
	size = sf::Vector2f(0, 0);
	rectangle = sf::RectangleShape(size);
	pos = sf::Vector2f(0, 0);
	rectangle.setFillColor(sf::Color(0, 0, 0));
	solid = false;
}

Wall::~Wall()
{
}

sf::RectangleShape Wall::getRectangle()
{
	return rectangle;
}

void Wall::update()
{
	rectangle.setPosition(pos);
}

bool Wall::isSolid()
{
	return solid;
}