#include "Wall.h"
#include "Util.h"

Wall::Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Vector2f speedmult, sf::Color color)
{
	extern Util resources;
	size = boxSize;
	pos = position;
	tex = resources.getTexture("ground.png");
	tex->setRepeated(true);
	sprite.setTexture(*tex);
	sprite.setColor(color);
	sprite.setScale(size.x / tex->getSize().x, size.y / tex->getSize().y);
	solid = true;
	sprite.setPosition(pos);
	speed = sf::Vector2f(0, 0);
	spd = speedmult;
	timer = 0;
	inview = true;
	refresh = 0;
}

Wall::Wall() {
	size = sf::Vector2f(0, 0);
	pos = sf::Vector2f(0, 0);
	solid = false;
}

Wall::~Wall()
{
}

sf::Sprite& Wall::getSprite(){
	return sprite;
}

sf::Vector2f Wall::OnCollision() {
	return speed;
}

bool Wall::inRect(sf::FloatRect rect)
{
	if (refresh<0) {
		refresh = 10;
		if ((pos.x > rect.left&&pos.x<rect.left + rect.width
			|| pos.x<rect.left&&pos.x + size.x>rect.left)
			&&(pos.y>rect.top&&pos.y < rect.top + rect.height
			|| pos.y<rect.top&&pos.y + size.y>rect.top))
			inview = true;
		else
			inview = false;
	}
	return inview;
}

void Wall::update()
{
	extern float gameSpeed;
	float fpsmult = 60.0f / gameSpeed;
	sprite.setTexture(*tex);
	int cycle = (int)timer % 200 - 99;
	speed.y = (std::min(cycle,-cycle)+50)*spd.y*fpsmult;
	speed.x = (std::min(cycle, -cycle) + 50)*spd.x*fpsmult;
	timer += fpsmult;
	refresh -= fpsmult;
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

bool Wall::visible()
{
	return inview;
}
