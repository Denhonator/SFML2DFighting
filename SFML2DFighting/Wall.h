#pragma once
#include <SFML/Graphics.hpp>
class Wall
{
public:
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f spd;
	Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Vector2f speedmult, sf::Color color);
	Wall();
	~Wall();
	sf::Sprite getSprite();
	sf::Vector2f OnCollision();
	void update();
	bool isSolid();
private:
	sf::Texture tex;
	sf::Sprite sprite;
	sf::Vector2f speed;
	int timer;
	bool solid;
};

