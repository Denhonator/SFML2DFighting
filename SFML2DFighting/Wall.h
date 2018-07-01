#pragma once
#include <SFML/Graphics.hpp>
class Wall
{
public:
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f speed;
	Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Color color);
	Wall();
	~Wall();
	sf::Sprite getSprite();
	void update();
	bool isSolid();
private:
	sf::Texture tex;
	sf::Sprite sprite;
	int timer;
	bool solid;
};

