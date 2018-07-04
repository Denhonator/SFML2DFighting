#include "Fighter.h"
#include "Wall.h"
#include <Windows.h>
#include "Util.h"

Fighter::Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet, int i, sf::Color color)
{
	extern Util resources;
	id = i;
	size = boxSize;
	normalSize = size;
	duckSize = sf::Vector2f(normalSize.x, normalSize.y*0.7f);
	groundSize = sf::Vector2f(normalSize.x*1.5f, normalSize.y*0.4f);
	pos = position;
	onGroundPos = pos;
	texture = resources.getTexture(spritesheet);
	fighterName = spritesheet.substring(0, spritesheet.find("."));
	speed = sf::Vector2f(0, 0);
	prevSpeed = sf::Vector2f(0, 0);
	platformSpeed = sf::Vector2f(0, 0);
	movementSpeed = 4;
	maxSpeed = sf::Vector2f(20, 30);
	sprite.setPosition(pos);
	state = "idle";
	health = 200;
	airtime = 0;
	lives = 999999;
	substate = 0;
	prevState = "idle";
	onGround = false;
	frame = -1;
	defineSprites();
	sprite.setTexture(*texture);
	sprite.setColor(color);
	ai = AI(id);
}

Fighter::Fighter()
{
}


Fighter::~Fighter()
{
}

sf::Sprite& Fighter::getSprite()
{
	return sprite;
}

sf::Texture* Fighter::getTexture()
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

sf::Vector2f Fighter::getGroundPos()
{
	return onGroundPos;
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
		speed = hit.speed*(60.0f/gameSpeed)+platformSpeed;
		platformSpeed = sf::Vector2f(0, 0);
		health -= hit.damage;
		state = "fall";
		frame = -1;
	}
}

int Fighter::getHealth()
{
	return health;
}

bool Fighter::isFlipped()
{
	return flip;
}

void Fighter::physics(std::vector<Wall*> wall, sf::String inputMethod)
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
	else if (std::abs(speed.x) < 0.1f && onGround && size == normalSize && state.find("attack") == sf::String::InvalidPos && (state != "fall"))
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
		if (canJump) {
			speed.y = -7*fpsmult;
			canJump = false;
			jumpHold = 20;
			state = "jump";
		}
		else if (doubleJump) {
			platformSpeed = sf::Vector2f(0, 0);
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

	speed += platformSpeed;
	prevSpeed = speed;
	speed = Fighter::collision(wall);

	pos += speed;

	speed -= platformSpeed;

	if (!onGround&&speed.y > 1) {
		airtime += fpsmult;
		if (airtime > 90)
			health = -999;
	}
	else
		airtime = 0;

	if (losecontrol > 0) {	//bounce when no control
		float bouncecap = 10.0f*fpsmult;
		platformSpeed = sf::Vector2f(0, 0);
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

sf::Vector2f Fighter::collision(std::vector<Wall*> wall)
{
	sf::Vector2f spd = speed;
	bool col = false;
	onGround = false;
	for (int i = 0; i<wall.size(); i++) {
		if (!wall.at(i)->visible())
			continue;
		//std::printf("%f, %f\n", pos.x, pos.y);
		if ((pos.x + size.x) > wall.at(i)->pos.x&&pos.x < (wall.at(i)->pos.x + wall.at(i)->size.x))
			if (wall.at(i)->pos.y + wall.at(i)->size.y / 2 > pos.y && speed.y >= 0)
				speed.y = std::fmin(speed.y, wall.at(i)->pos.y - (pos.y + size.y));
			else if ((wall.at(i)->pos.y + wall.at(i)->size.y / 2)<pos.y&&speed.y < 0)
				speed.y = std::fmax(speed.y, (wall.at(i)->pos.y + wall.at(i)->size.y) - pos.y);

		if (pos.y<(wall.at(i)->pos.y + wall.at(i)->size.y) && (pos.y + size.y)>wall.at(i)->pos.y) {
			if (speed.x > 0 && wall.at(i)->pos.x>pos.x) {
				speed.x = std::fmin(speed.x, wall.at(i)->pos.x - (pos.x + size.x));
			}
			if (speed.x < 0 && (wall.at(i)->pos.x + wall.at(i)->size.x)<pos.x + 5) {
				speed.x = std::fmax(speed.x, (wall.at(i)->pos.x + wall.at(i)->size.x) - pos.x);
			}
		}
		if (speed.x != spd.x || speed.y != spd.y) {
			col = true;
			platformSpeed = wall.at(i)->OnCollision();	//Moving platform collision
			if (speed.y < 0 && speed.y < spd.y) {
				pos.y = wall.at(i)->pos.y - size.y;
				if (losecontrol > 0)
					speed.y = platformSpeed.y * 2;
				else
					speed.y = 0;
			}
			if (wall.at(i)->pos.y >= pos.y + size.y) {
				onGroundPos = pos;
				onGround = true;
				canJump = true;
				doubleJump = false;
				doublejumps = 0;
			}
			spd = speed;
		}
	}
	//std::printf("On ground: %d\n", onGround);
	collided = col;
	return speed;
}

sf::String Fighter::chosenAction(sf::String inputMethod)
{
	sf::String action = "";
	pressedkey = false;
	if (losecontrol > 0)
		return "";
	if (inputMethod == "WASD") {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			action += "A";
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))	//can't move in both directions at once
			action += "D";
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			action += "W";
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			action += "S";
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
			action += "F";
	}
	else if (inputMethod == "ARROWS") {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			action += "A";
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))	//can't move in both directions at once
			action += "D";
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			action += "W";
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			action += "S";
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
			action += "F";
	}
	else if (inputMethod == "AI") {
		ai.run = true;
		action = ai.getKeys();
	}
	if (action != "")
		pressedkey = true;
	return action;
}

void Fighter::animate()
{
	sf::Vector2f offset = sf::Vector2f(0, 0);
	if (state != prevState)
		frame = -1;

	//State control
	//--------------------------------------------------------------------------------------------
	if (state == "idle") {
		if (frame < 0) {
			substate = 0;
			frame = 60;
		}
		if (frame < 30)
			substate = 1;
	}
	else if (state == "run") {
		if (frame < 1) {
			if (substate >= 4 || state != prevState)
				substate = 0;
			frame = 16;
			substate++;
		}
	}
	else if (state == "jump") {
		if (doublejumps % 2 == 0)
			substate = 1;
		else
			substate = 0;
	}
	else if (state == "duck") {
		substate = 0;
	}
	else if (state == "fall") {
		if (onGround) {
			if (substate < 4)
				substate = 4;
			else if (substate == 4)
				substate = 5;
		}
		else if (substate == 5) {
			substate = 4;
		}
		if (frame < 0 && prevState != state) {
			frame = 7;
			substate = 0;
		}
		else if (frame < 0) {
			if (substate < 4) {
				substate++;
				frame = 7;
			}
		}
		if (substate > 3)
			offset = sf::Vector2f(0, -1.20*(substate - 2));
	}
	else if (state == "normalattack") {
		if (frame < 0) {
			frame = 25;
			if (prevState == state)
				frame = 30;
			substate = 0;
		}
		if (state == "normalattack") {
			if (frame < 25)
				substate = 0;
			if (frame < 18)
				substate = 1;
			if (frame < 10)
				substate = 0;
			if (frame < 5)
				substate = 2;
			if (substate == 0)
				offset = sf::Vector2f(0, 5);
			if (substate == 1)
				offset = sf::Vector2f(5, 5);
		}
	}
	else if (state == "groundattack") {
		if (frame < 0) {
			frame = 30;
			if (prevState == state)
				frame = 45;
			substate = 0;
		}
		if (state == "groundattack") {
			if (frame < 20)
				substate = 1;
			if (frame < 10)
				substate = 2;
		}
		if (substate == 0)
			offset = sf::Vector2f(-11, -10);
		if (substate == 1)
			offset = sf::Vector2f(9, -7);
		if (substate == 2)
			offset = sf::Vector2f(12.5f, 9);
	}
	else if (state == "airattack") {
		if (frame < 0) {
			frame = 40;
			substate = 0;
		}
		if (onGround) {
			substate = 1;
			frame = std::min(frame, 10.0f);
		}
		else {
			if (frame < 33)
				substate = 1;
			if (frame < 26)
				substate = 2;
			if (frame < 10)
				substate = 1;
		}
		if (substate < 2)
			offset = sf::Vector2f(3, -7);
		else
			offset = sf::Vector2f(8, 0);
	}

	//Hitbox Definitions
	//-------------------------------------------------------------------------------------------------------
	hitbox.clear();
	if (state == "normalattack"&&substate == 1) {
		Hitbox addbox = Hitbox(sf::Vector2f(30, 30), pos);
		addbox.damage = 15;
		addbox.iframe = 20;
		addbox.losecontrol = 20;
		addbox.owner = id;
		addbox.speed.y = -10;
		if (flip) {
			addbox.pos += sf::Vector2f(-45, 15);
			addbox.speed.x = -7;
		}
		else {
			addbox.pos += sf::Vector2f(65, 15);
			addbox.speed.x = 7;
		}
		addbox.update();
		hitbox.push_back(addbox);
	}
	else if (state == "airattack"&&substate == 2) {
		Hitbox addbox = Hitbox(sf::Vector2f(35, 35), pos);
		addbox.damage = 10;
		addbox.iframe = 20;
		addbox.losecontrol = 20;
		addbox.owner = id;
		addbox.speed.y = 10;
		if (flip) {
			addbox.pos += sf::Vector2f(-70, 0);
			addbox.speed.x = -4;
		}
		else {
			addbox.pos += sf::Vector2f(87.5f, 0);
			addbox.speed.x = 4;
		}
		addbox.update();
		hitbox.push_back(addbox);
	}
	else if (state == "groundattack"&&substate == 1) {
		Hitbox addbox = Hitbox(sf::Vector2f(50, 40), pos);
		addbox.damage = 15;
		addbox.iframe = 20;
		addbox.losecontrol = 20;
		addbox.owner = id;
		addbox.speed.y = 10;
		if (flip) {
			addbox.pos += sf::Vector2f(-100, 45);
			addbox.speed.x = -4;
		}
		else {
			addbox.pos += sf::Vector2f(110, 45);
			addbox.speed.x = 4;
		}
		addbox.update();
		hitbox.push_back(addbox);
	}

	//Set sprite with possible flip and scaling
	//-------------------------------------------------------------------------------------------------------
	sprite.setTexture(*texture);
	sprite.setTextureRect(spriteDefinitions[state + std::to_string(substate)]);
	if (flip) {
		sprite.setOrigin(sprite.getTextureRect().width, 0);
		sprite.setScale(-3, 3);
		offset.x *= -3;
		offset.y *= 3;
	}
	else {
		sprite.setOrigin(0, 0);
		sprite.setScale(3, 3);
		offset.x *= 3;
		offset.y *= 3;
	}
	sprite.setPosition(sf::Vector2f(pos.x + offset.x - (((sprite.getTextureRect().width * 3) - size.x) / 2), pos.y + offset.y - 5));
	extern float gameSpeed;
	frame -= 60.0f / gameSpeed;
	prevState = state;
	//std::cout << std::string(state) + std::to_string(substate) << std::endl;
}
