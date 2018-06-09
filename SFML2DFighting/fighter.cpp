#include "Fighter.h"
#include "Wall.h"
#include <iostream>

Fighter::Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet, int i)
{
	id = i;
	size = boxSize;
	normalSize = size;
	duckSize = sf::Vector2f(normalSize.x, normalSize.y*0.7f);
	groundSize = sf::Vector2f(normalSize.x*1.5f, normalSize.y*0.4f);
	pos = position;
	if (!texture.loadFromFile(spritesheet))
		std::printf("Could not find %s", spritesheet);
	fighterName = spritesheet.substring(0, spritesheet.find("."));
	speed = sf::Vector2f(0, 0);
	prevSpeed = sf::Vector2f(0, 0);
	movementSpeed = 4;
	maxSpeed = sf::Vector2f(20, 30);
	sprite.setPosition(pos);
	state = "idle";
	substate = 0;
	prevState = "idle";
	onGround = false;
	frame = -1;
	defineSprites();
	sprite.setTexture(texture);
}

Fighter::Fighter()
{
}


Fighter::~Fighter()
{
}

sf::Sprite Fighter::getSprite()
{
	return sprite;
}

sf::Texture Fighter::getTexture()
{
	return texture;
}

std::vector<Hitbox> Fighter::getHitboxes()
{
	return hitbox;
}

void Fighter::getHit(Hitbox hit)
{
	if (iframe == 0) {
		std::cout << "Fighter " << id << " was hit for " << hit.damage << " damage by fighter " << hit.owner << std::endl;
		iframe = hit.iframe;
		losecontrol = hit.losecontrol;
		speed = hit.speed;
		state = "fall";
		frame = -1;
	}
}

void Fighter::physics(std::vector<Wall> wall, sf::String inputMethod)
{
	sf::String input = Fighter::chosenAction(inputMethod);

//Moving
//----------------------------------------------------------------------
	if (input.find("D")!=sf::String::InvalidPos) {
		if (state != "duck"&&state!="normalattack") {
			if (speed.x < maxSpeed.x) {
				speed.x += movementSpeed;
			}
			if (onGround)
				state = "run";
		}
		flip = false;
	}
	if (input.find("A") != sf::String::InvalidPos) {
		if (state != "duck"&&state != "normalattack") {
			if (speed.x > -maxSpeed.x) {
				speed.x -= movementSpeed;
			}
			if (onGround)
				state = "run";
		}
		flip = true;
	}
//Jumping
//----------------------------------------------------------------------
	if (input.find("W") != sf::String::InvalidPos&&state!="duck"&&state != "normalattack") {
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
	if (!onGround && (state == "idle" || state == "run" || state == "duck")) {
		if (canJump) {
			canJump = false;
			doubleJump = true;
		}
		state = "jump";
	}
//Ducking
//----------------------------------------------------------------------
	if (input.find("S") != sf::String::InvalidPos&&onGround&&state != "normalattack") {
		if(size!=duckSize)
			pos.y += normalSize.y - duckSize.y;
		size = duckSize;
		state = "duck";
	}
	else if(size==duckSize) {
		size = normalSize;
		pos.y -= normalSize.y - duckSize.y;
		state = "idle";
	}
//Attacking
//----------------------------------------------------------------------
	if (input.find("F") != sf::String::InvalidPos&&onGround) {
		if(onGround)
			state = "normalattack";
	}
	if (iframe > 0)
		iframe--;

//Idle
//----------------------------------------------------------------------
	if (state == "fall"&&losecontrol == 0 && pressedkey)
		state = "idle";
	else if (speed.x == 0 && speed.y == 0 && collided && state != "duck" && state != "normalattack" && (state != "fall")) {
		state = "idle";
	}
//Ground size
//----------------------------------------------------------------------
	if (state == "fall"&&substate > 2) {
		if (onGround&&size != groundSize)
			speed.y = prevSpeed.y;
		size = groundSize;
	}
	else if (size == groundSize) {
		size = normalSize;
		pos.y -= normalSize.y - groundSize.y;
	}
//Modify speed and position
//----------------------------------------------------------------------
	if (speed.y<maxSpeed.y) {
		speed.y += movementSpeed * 0.2f;
	}
	if (losecontrol > 0)
		losecontrol--;
	if(losecontrol==0)
		speed.x *= 0.7f;
	if (abs(speed.x) < 0.01f)
		speed.x = 0;

	prevSpeed = speed;
	speed = Fighter::collision(wall, speed, pos, size);

	pos.x += speed.x;
	pos.y += speed.y;

//Animate
//----------------------------------------------------------------------
	Fighter::animate();
	//std::printf("%f, %f\n", pos.x, pos.y);
}
