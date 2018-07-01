#pragma once
#include "Hitbox.h"
#include "AI.h"
#include "Wall.h"

class Fighter
{
public:
	sf::Vector2f size;
	sf::Vector2f pos;
	sf::Vector2f speed;
	AI ai;
	Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet, int i, sf::Color color=sf::Color(255,255,255,255));
	Fighter();
	~Fighter();
	sf::Sprite getSprite();
	sf::Texture getTexture();
	std::vector<Hitbox> getHitboxes();
	sf::String getState();
	float getFrame();
	void getHit(Hitbox hit);
	int getHealth();
	void physics(std::vector<Wall*> wall, sf::String inputMethod);

	sf::Vector2f collision(std::vector<Wall*> wall) {
		sf::Vector2f spd = speed;
		bool col = false;
		onGround = false;
		for (int i = 0; i<wall.size() ; i++) {
			//std::printf("%f, %f\n", pos.x, pos.y);
			if ((pos.x + size.x) > wall.at(i)->pos.x&&pos.x < (wall.at(i)->pos.x + wall.at(i)->size.x))
				if (wall.at(i)->pos.y+wall.at(i)->size.y/2 > pos.y && speed.y >= 0)
					speed.y = std::fmin(speed.y, wall.at(i)->pos.y - (pos.y + size.y));
				else if ((wall.at(i)->pos.y+wall.at(i)->size.y/2)<pos.y&&speed.y < 0)
					speed.y = std::fmax(speed.y, (wall.at(i)->pos.y + wall.at(i)->size.y) - pos.y);

			if (pos.y<(wall.at(i)->pos.y + wall.at(i)->size.y)&&(pos.y + size.y)>wall.at(i)->pos.y) {
				if (speed.x > 0 && wall.at(i)->pos.x>pos.x) {
					speed.x = std::fmin(speed.x, wall.at(i)->pos.x - (pos.x+size.x));
				}
				if (speed.x < 0 && (wall.at(i)->pos.x+wall.at(i)->size.x)<pos.x+5) {
					speed.x = std::fmax(speed.x, (wall.at(i)->pos.x + wall.at(i)->size.x) - pos.x);
				}
			}
			if (speed.x != spd.x || speed.y != spd.y) {
				col = true;
				platformSpeed = wall.at(i)->OnCollision();
				if (speed.y < 0 && speed.y < spd.y) {
					pos.y = wall.at(i)->pos.y-size.y;
					if (losecontrol > 0)
						speed.y = platformSpeed.y*2;
					else
						speed.y =  0;
				}
				if (wall.at(i)->pos.y>=pos.y+size.y) {
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

	void animate() {
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
				if(substate>=4||state!=prevState)
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
			if (frame < 0 && prevState!=state) {
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
				offset = sf::Vector2f(0, -1.20*(substate-2));
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
		sprite.setTexture(texture);
		sprite.setTextureRect(spriteDefinitions[state + std::to_string(substate)]);
		if (flip) {
			sprite.setOrigin(sprite.getTextureRect().width, 0);
			sprite.setScale(-3,3);
			offset.x *= -3;
			offset.y *= 3;
		}
		else {
			sprite.setOrigin(0, 0);
			sprite.setScale(3, 3);
			offset.x *= 3;
			offset.y *= 3;
		}
		sprite.setPosition(sf::Vector2f(pos.x+offset.x-(((sprite.getTextureRect().width*3)-size.x)/2), pos.y+offset.y-5));
		extern float gameSpeed;
		frame-=60.0f/gameSpeed;
		prevState = state;
		//std::cout << std::string(state) + std::to_string(substate) << std::endl;
	}
//Input
//-------------------------------------------------------------------------------------------------------
	sf::String chosenAction(sf::String inputMethod) {
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


	void defineSprites() {
		//std::cout << std::string(fighterName) << std::endl;
		if (fighterName == "toad") {
			spriteDefinitions["idle0"] = sf::IntRect(36, 23, 25, 35);
			spriteDefinitions["idle1"] = sf::IntRect(69, 23, 25, 35);
			spriteDefinitions["duck0"] = sf::IntRect(388, 29, 41, 26);
			spriteDefinitions["run0"] = sf::IntRect(437, 26, 28, 33);
			spriteDefinitions["run1"] = sf::IntRect(467, 26, 25, 33);
			spriteDefinitions["run2"] = sf::IntRect(499, 30, 33, 30);
			spriteDefinitions["run3"] = sf::IntRect(536, 26, 21, 36);
			spriteDefinitions["run4"] = sf::IntRect(561, 30, 33, 30);
			spriteDefinitions["jump0"] = sf::IntRect(499, 30, 33, 30);
			spriteDefinitions["jump1"] = sf::IntRect(561, 30, 33, 30);
			spriteDefinitions["normalattack0"] = sf::IntRect(99, 25, 26, 31);
			spriteDefinitions["normalattack1"] = sf::IntRect(128, 25, 38, 31);
			spriteDefinitions["normalattack2"] = sf::IntRect(36, 23, 25, 35);
			//spriteDefinitions["airattack0"] = sf::IntRect(168, 26, 34, 31);
			spriteDefinitions["airattack0"] = sf::IntRect(209, 17, 34, 41);
			spriteDefinitions["airattack1"] = sf::IntRect(250, 19, 33, 38);
			spriteDefinitions["airattack2"] = sf::IntRect(285, 24, 51, 32);
			spriteDefinitions["groundattack0"] = sf::IntRect(346, 67, 37, 44);
			spriteDefinitions["groundattack1"] = sf::IntRect(391, 70, 66, 41);
			spriteDefinitions["groundattack2"] = sf::IntRect(463, 86, 51, 25);
			spriteDefinitions["fall0"] = sf::IntRect(542, 160, 27, 36);
			spriteDefinitions["fall1"] = sf::IntRect(510, 162, 28, 37);
			spriteDefinitions["fall2"] = sf::IntRect(84, 164, 33, 34);
			spriteDefinitions["fall3"] = sf::IntRect(402, 218, 38, 28);
			spriteDefinitions["fall4"] = sf::IntRect(120, 174, 41, 20);
			//spriteDefinitions["fall5"] = sf::IntRect(447, 127, 33, 27);
			spriteDefinitions["fall5"] = sf::IntRect(549, 131, 38, 21);
		}
	}
private:
	int id;
	bool pressedkey = false;
	sf::Vector2f normalSize;
	sf::Vector2f duckSize;
	sf::Vector2f groundSize;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String state;
	int substate;
	sf::String prevState;
	sf::String fighterName;
	bool flip = false;
	float frame;
	float iframe = 0;
	float losecontrol = 0;
	int health;
	int lives;
	float movementSpeed;
	sf::Vector2f maxSpeed;
	sf::Vector2f prevSpeed;
	sf::Vector2f platformSpeed;
	bool canJump = false;
	bool doubleJump = false;
	bool collided = false;
	bool onGround = false;
	int doublejumps = 0;
	float jumpHold = 0;
	std::map <sf::String, sf::IntRect> spriteDefinitions;
	std::vector<Hitbox> hitbox;
};