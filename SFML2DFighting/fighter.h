#pragma once
#include <SFML/Graphics.hpp>
#include "Wall.h"
#include <iostream>
class Fighter
{
public:
	sf::Vector2f pos;
	sf::Vector2f speed;
	Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet);
	~Fighter();
	sf::Sprite getSprite();
	void physics(Wall wall[], sf::String inputMethod);

	sf::Vector2f collision(Wall wall[], sf::Vector2f speed, sf::Vector2f pos, sf::Vector2f size) {
		sf::Vector2f spd = speed;
		bool col = false;
		onGround = false;
		for (int i = 0; wall[i].isSolid(); i++) {
			//std::printf("%f, %f\n", pos.x, pos.y);
			if ((pos.x + size.x) > wall[i].pos.x&&pos.x < (wall[i].pos.x + wall[i].size.x))
				if (wall[i].pos.y > (pos.y + size.y - 5) && speed.y >= 0)
					speed.y = std::fmin(speed.y, wall[i].pos.y - (pos.y + size.y));
				else if ((wall[i].pos.y+wall[i].size.y-5)<pos.y&&speed.y < 0)
					speed.y = std::fmax(speed.y, (wall[i].pos.y + wall[i].size.y) - pos.y);

			if (pos.y<(wall[i].pos.y + wall[i].size.y)&&(pos.y + size.y)>wall[i].pos.y) {
				if (speed.x > 0 && wall[i].pos.x>pos.x) {
					speed.x = std::fmin(speed.x, wall[i].pos.x - (pos.x+size.x));
				}
				if (speed.x < 0 && (wall[i].pos.x+wall[i].size.x)<pos.x+5) {
					speed.x = std::fmax(speed.x, (wall[i].pos.x + wall[i].size.x) - pos.x);
				}
			}
			if (speed.x != spd.x || speed.y != spd.y) {
				col = true;
				if (speed.y == 0 && spd.y > 0) {
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

//Animation control
//--------------------------------------------------------------------------------------------
		if (state == "idle") {
			if (frame < 0) {
				substate = 0;
				frame = 60;
			}
			if (frame == 30)
				substate = 1;		
		}
		else if (state == "run") {
			if (frame < 0) {
				substate = 0;
				if (state != prevState)
					frame = 74;
				else
					frame = 60;
			}
			if (frame % 15 == 0&&substate<4)
				substate++;
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
		else if (state == "normalattack") {
			if (frame < 0&&prevState!=state) {	//Get to attacking only if state has changed
				frame = 30;
				substate = 0;
			}
			else if (frame < 0) {	//If attack is ending, go to idle before attacking again
				state = "idle";
				substate = 0;
			}
			if (state == "normalattack") {
				if (frame == 30 || frame == 10)
					substate = 0;
				else if (frame == 20)
					substate = 1;
				else if (frame == 5)
					substate = 2;
				if (substate == 0)
					offset = sf::Vector2f(0, 5);
				if (substate == 1)
					offset = sf::Vector2f(5, 5);
			}
		}

//Set sprite with possible flip and scaling
//-------------------------------------------------------------------------------------------------------
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
		frame--;
		prevState = state;
		std::cout << std::string(state) + std::to_string(substate) << std::endl;
	}


	sf::String chosenAction(sf::String inputMethod) {
		sf::String action = "";
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

		}
	}

private:
	sf::Vector2f size;
	sf::Vector2f normalSize;
	sf::Vector2f duckSize;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String state;
	int substate;
	sf::String prevState;
	sf::String fighterName;
	bool flip = false;
	int frame;
	float movementSpeed;
	sf::Vector2f maxSpeed;
	bool canJump = false;
	bool doubleJump = false;
	bool collided = false;
	bool onGround = false;
	int doublejumps = 0;
	float jumpHold = 0;
	std::map <sf::String, sf::IntRect> spriteDefinitions;
};

