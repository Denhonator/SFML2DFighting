#include "Wall.h"

Wall::Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Vector2f speedmult, sf::Color color)
{
	size = boxSize;
	pos = position;
	tex.loadFromFile("ground.png");
	tex.setRepeated(true);
	sprite.setTexture(tex);
	sprite.setColor(color);
	sprite.setScale(size.x / tex.getSize().x, size.y / tex.getSize().y);
	solid = true;
	sprite.setPosition(pos);
	speed = sf::Vector2f(0, 0);
	spd = speedmult;
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

sf::Sprite Wall::getSprite(){
	return sprite;
}

sf::Vector2f Wall::OnCollision() {
	return speed;
}

void Wall::update()
{
	sprite.setTexture(tex);
	int cycle = (int)timer % 200 - 99;
	speed.y = (std::min(cycle,-cycle)+50)*spd.y;
	speed.x = (std::min(cycle, -cycle) + 50)*spd.x;
	timer += 1;
	if (speed.x != 0 || speed.y != 0) {
		_RPT1(0, "%d\n", (std::min(cycle, -cycle) + 50));
		pos += speed;
		sprite.setPosition(pos);
	}
}

bool Wall::isSolid()
{
	return solid;
}