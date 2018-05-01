#include "Fighter.h"
#include "Wall.h"


Fighter::Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet)
{
	size = boxSize;
	pos = position;
	if (!texture.loadFromFile(spritesheet))
		std::printf("Could not find %s", spritesheet);
	speed = sf::Vector2f(0, 0);
	movementSpeed = 4;
	maxSpeed = sf::Vector2f(20, 30);
	sprite.setPosition(pos);
	state = "idle";
	prevState = "idle";
	onGround = false;
	frame = -1;
	sprite.setTexture(texture);
}


Fighter::~Fighter()
{
}

sf::Sprite Fighter::getSprite()
{
	return sprite;
}

void Fighter::physics(Wall wall[])
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (speed.x < maxSpeed.x) {
			speed.x += movementSpeed;
		}
		if(onGround)
			state = "run";
		flip = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (speed.x > -maxSpeed.x) {
			speed.x -= movementSpeed;
		}
		if(onGround)
			state = "run";
		flip = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (speed.y == 0&&canJump) {
			speed.y = -7;
			canJump = false;
			jumpHold = 20;
			state = "jump";
		}
		else if (doubleJump) {
			speed.y = -10;
			doubleJump = false;
			doublejumps++;
		}
		else if (jumpHold > 0) {
			speed.y -= jumpHold*0.10f;
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

	if (speed.x == 0 && speed.y == 0 && collided) {
		state = "idle";
	}
	Fighter::animate();

	//std::printf("%f, %f\n", pos.x, pos.y);
}
