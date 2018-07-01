#include "Fighter.h"
#include "Wall.h"

Fighter::Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet, int i, sf::Color color)
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
	health = 200;
	lives = 999999;
	substate = 0;
	prevState = "idle";
	onGround = false;
	frame = -1;
	defineSprites();
	sprite.setTexture(texture);
	sprite.setColor(color);
	ai = AI(id);
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

sf::String Fighter::getState()
{
	return state;
}

float Fighter::getFrame()
{
	return frame;
}

void Fighter::getHit(Hitbox hit)
{
	if (iframe <= 0) {
		extern float gameSpeed;
		iframe = hit.iframe;
		losecontrol = hit.losecontrol;
		speed = hit.speed*(60.0f/gameSpeed);
		health -= hit.damage;
		state = "fall";
		frame = -1;
	}
}

int Fighter::getHealth()
{
	return health;
}

void Fighter::physics(std::vector<sf::FloatRect> wall, sf::String inputMethod)
{
	sf::String input = Fighter::chosenAction(inputMethod);
	extern float gameSpeed;
	float fpsmult = (60.0f / gameSpeed);
	float ms = movementSpeed * fpsmult;
//Idle
//----------------------------------------------------------------------
	if (state == "fall"&&losecontrol == 0 && pressedkey)
		state = "idle";
	else if (frame < 0 && state != "fall")
		state = "idle";
	else if (std::abs(speed.x) < 0.1f && speed.y == 0 && collided && size == normalSize && state.find("attack") == sf::String::InvalidPos && (state != "fall"))
		state = "idle";
	if (state == "idle"&&prevState != state)
		substate = 0;
//Moving
//----------------------------------------------------------------------
	if (input.find("D")!=sf::String::InvalidPos) {
		if (size==normalSize&&state!="normalattack" && state != "groundattack") {
			if (speed.x < maxSpeed.x) {
				speed.x += ms;
			}
			if (onGround)
				state = "run";
		}
		flip = false;
	}
	if (input.find("A") != sf::String::InvalidPos) {
		if (size==normalSize&&state != "normalattack" && state != "groundattack") {
			if (speed.x > -maxSpeed.x) {
				speed.x -= ms;
			}
			if (onGround)
				state = "run";
		}
		flip = true;
	}
//Jumping
//----------------------------------------------------------------------
	if (input.find("W") != sf::String::InvalidPos&&state!="duck"&&state.find("attack")==sf::String::InvalidPos) {
		if (speed.y == 0&&canJump) {
			speed.y = -7*fpsmult;
			canJump = false;
			jumpHold = 20;
			state = "jump";
		}
		else if (doubleJump) {
			speed.y = -10 * fpsmult;
			doubleJump = false;
			doublejumps++;
		}
		else if (jumpHold > 0) {
			speed.y -= jumpHold*0.10f*fpsmult*fpsmult;
			jumpHold-= jumpHold*0.15f*fpsmult;
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
//Attacking
//----------------------------------------------------------------------
	if (input.find("F") != sf::String::InvalidPos&&state.find("attack")==sf::String::InvalidPos) {
		if (onGround) {
			if (input.find("S") != sf::String::InvalidPos)
				state = "groundattack";
			else
				state = "normalattack";
		}
		else
			state = "airattack";
	}
	extern float gameSpeed;
	if (iframe > 0)
		iframe-=60.0f/gameSpeed;

//Ducking
//----------------------------------------------------------------------
	if (input.find("S") != sf::String::InvalidPos&&onGround&&state.find("attack")==sf::String::InvalidPos) {
		if (size == normalSize)
			pos.y += normalSize.y - duckSize.y;
		else if (size == groundSize)
			pos.y += groundSize.y - duckSize.y;
		size = duckSize;
		state = "duck";
	}
	else if (size == duckSize) {
		size = normalSize;
		pos.y -= normalSize.y - duckSize.y;
		if(state=="duck")
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
		speed.y += ms * 0.2f * fpsmult;
	}
	if (losecontrol > 0)
		losecontrol-=fpsmult;
	if(losecontrol<=0)
		speed.x *= 0.7f;
	if (abs(speed.x) < 0.01f)
		speed.x = 0;

	prevSpeed = speed;
	speed = Fighter::collision(wall);

	pos.x += speed.x;
	pos.y += speed.y;

	extern sf::FloatRect view;
	if (pos.y > view.top + view.height)
		health = -999;

	if (losecontrol > 0) {	//bounce when no control
		float bouncecap = 10.0f*fpsmult;
		if(speed.x!=prevSpeed.x)
			speed.x = -std::max(std::min(prevSpeed.x, bouncecap), -bouncecap);
		if (speed.y != prevSpeed.y)
			speed.y = -std::max(std::min(prevSpeed.y, bouncecap),-bouncecap);
	}

//Animate
//----------------------------------------------------------------------
	//std::cout << std::string(state) << substate << ", " << frame << "\n";
	Fighter::animate();
	//std::printf("%f, %f\n", pos.x, pos.y);
}
