#include "Wall.h"

Wall::Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Color color)
{
	size = boxSize;
	pos = position;
	tex.loadFromFile("ground.png");
	tex.setRepeated(true);
	sprite.setTexture(tex);
	sprite.setTextureRect(sf::IntRect(0,0, size.x, size.y));
	sprite.setColor(color);
	solid = true;
	sprite.setPosition(pos);
	speed.x = 0;
	speed.y = 0;
	timer = 0;
}

Wall::Wall() {
	size = sf::Vector2f(0, 0);
	pos = sf::Vector2f(0, 0);
	solid = false;
}

Wall::~Wall()
{
}

sf::Sprite Wall::getSprite()
{
	return sprite;
}

void Wall::update()
{
	sprite.setTexture(tex);
	int cycle = (int)timer % 201 - 100;
	speed.x = (std::min(cycle,-cycle)+50)*0.1f;
	timer += 1;
	if (speed.x != 0 || speed.y != 0) {
		pos += speed;
		sprite.setPosition(pos);
	}
}

bool Wall::isSolid()
{
	return solid;
}