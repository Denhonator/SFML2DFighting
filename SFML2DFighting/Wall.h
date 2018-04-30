#pragma once
#include <SFML/Graphics.hpp>
class Wall
{
public:
	sf::Vector2f pos;
	sf::Vector2f size;
	Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Color color);
	Wall();
	~Wall();
	sf::RectangleShape getRectangle();
	void update();
	bool isSolid();
private:
	sf::RectangleShape rectangle;
	bool solid;
};

