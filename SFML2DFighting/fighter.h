#pragma once
#include <SFML/Graphics.hpp>
#include "Wall.h"
class Fighter
{
public:
	sf::Vector2f pos;
	sf::Vector2f speed;
	Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::Color color);
	~Fighter();
	sf::RectangleShape getRectangle();
	void physics(Wall wall[]);

	sf::Vector2f collision(Wall wall[], sf::Vector2f speed, sf::Vector2f pos, sf::Vector2f size) {
		sf::Vector2f spd = speed;
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
				std::printf("Collision with %d\n", i);
				if (speed.y == 0 && spd.y > 0) {
					canJump = true;
					doubleJump = false;
					doublejumps = 0;
				}
				spd = speed;
			}
		}
		return speed;
	}

private:
	sf::Vector2f size;
	sf::RectangleShape rectangle;
	float movementSpeed;
	sf::Vector2f maxSpeed;
	bool canJump = false;
	bool doubleJump = false;
	int doublejumps = 0;
	float jumpHold = 0;
};

