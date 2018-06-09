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
	std::vector<int> hitcheck();
	void update();
	bool solid;
	int damage;
	int owner;
	int iframe;
	int losecontrol;
	sf::Vector2f speed;
private:
	sf::RectangleShape rectangle;
};

