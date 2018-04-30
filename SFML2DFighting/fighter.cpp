#include "Fighter.h"
#include "Wall.h"


Fighter::Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::Color color)
{
	size = boxSize;
	rectangle = sf::RectangleShape(boxSize);
	pos = position;
	speed = sf::Vector2f(0, 0);
	rectangle.setFillColor(color);
	movementSpeed = 5;
	maxSpeed = sf::Vector2f(20, 30);
	rectangle.setPosition(pos);
}


Fighter::~Fighter()
{
}

sf::RectangleShape Fighter::getRectangle()
{
	return rectangle;
}

void Fighter::physics(Wall wall[])
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (speed.x < maxSpeed.x) {
			speed.x += movementSpeed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (speed.x > -maxSpeed.x) {
			speed.x -= movementSpeed;
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (speed.y == 0&&canJump) {
			speed.y = -10;
			canJump = false;
			jumpHold = 20;
		}
		else if (doubleJump) {
			speed.y = -15;
			doubleJump = false;
			doublejumps++;
		}
		else if (jumpHold > 0) {
			speed.y -= jumpHold*0.15f;
			jumpHold-= jumpHold*0.15f;
		}
	}
	else if (!canJump&&doublejumps<1) {
		doubleJump = true;
		jumpHold = 10;
	}

	if (speed.y<maxSpeed.y) {
		speed.y += movementSpeed * 0.2f;
	}

	speed.x *= 0.7f;
	if (abs(speed.x) < 0.01f)
		speed.x = 0;

	speed = Fighter::collision(wall, speed, pos, size);

	pos.x += speed.x;
	pos.y += speed.y;

	rectangle.setPosition(pos);
	//std::printf("%f, %f\n", pos.x, pos.y);
}
