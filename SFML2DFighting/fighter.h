#pragma once
#include <SFML/Graphics.hpp>
#include "Wall.h"
class Fighter
{
public:
	sf::Vector2f pos;
	sf::Vector2f speed;
	Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet);
	~Fighter();
	sf::Sprite getSprite();
	void physics(Wall wall[]);

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
		if (frame < 0||(state!=prevState)) {
			if (state == "idle")
				frame = 60;
			if (state == "run") {
				if (state != prevState)
					frame = 75;
				else
					frame = 60;
			}
		}
		if (state == "idle") {
			if(frame==60)
				sprite.setTextureRect(sf::IntRect(36, 23, 25, 35));
			else if(frame==30)
				sprite.setTextureRect(sf::IntRect(69, 23, 25, 35));
		}
		else if (state == "run") {
			if(frame==75)
				sprite.setTextureRect(sf::IntRect(437, 26, 28, 33));
			else if (frame == 60)
				sprite.setTextureRect(sf::IntRect(467, 26, 25, 33));
			else if (frame == 45)
				sprite.setTextureRect(sf::IntRect(499, 30, 33, 30));
			else if (frame == 30)
				sprite.setTextureRect(sf::IntRect(536, 26, 21, 36));
			else if (frame == 15)
				sprite.setTextureRect(sf::IntRect(561, 30, 33, 30));
		}
		else if (state == "jump") {
			if(doublejumps%2==0)
				sprite.setTextureRect(sf::IntRect(499, 30, 33, 30));
			else
				sprite.setTextureRect(sf::IntRect(561, 30, 33, 30));
		}
		else if (state == "duck") {
			sprite.setTextureRect(sf::IntRect(388, 29, 41, 26));
		}
		if (flip) {
			sprite.setOrigin(sprite.getTextureRect().width, 0);
			sprite.setScale(-3,3);
		}
		else {
			sprite.setOrigin(0, 0);
			sprite.setScale(3, 3);
		}
		sprite.setPosition(sf::Vector2f(pos.x-(((sprite.getTextureRect().width*3)-size.x)/2), pos.y-5));
		frame--;
		prevState = state;
	}

private:
	sf::Vector2f size;
	sf::Vector2f normalSize;
	sf::Vector2f duckSize;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String state;
	sf::String prevState;
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
};

