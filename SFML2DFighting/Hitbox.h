#pragma once
#include <SFML/Graphics.hpp>
class Hitbox
{
public:
	sf::Vector2f pos;
	sf::Vector2f size;
	Hitbox(sf::Vector2f boxSize, sf::Vector2f position);
	Hitbox();
	~Hitbox();
	sf::RectangleShape getRectangle();
	void update();
	bool solid;
private:
	sf::RectangleShape rectangle;
};

