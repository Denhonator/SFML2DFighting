#pragma once
#include <SFML/Graphics.hpp>
class Wall
{
public:
	sf::Vector2f pos;
	sf::Vector2f size;
	sf::Vector2f spd;
	Wall(sf::Vector2f boxSize, sf::Vector2f position, sf::Vector2f speedmult, sf::Color color=sf::Color(255,255,255,255));
	Wall();
	~Wall();
	sf::Sprite& getSprite();
	sf::Vector2f OnCollision();
	bool inRect(sf::FloatRect rect);
	void update();
	bool isSolid();
	bool visible();
	bool global = false;
private:
	sf::Texture* tex;
	sf::Sprite sprite;
	sf::Vector2f speed;
	float timer;
	float refresh;
	bool solid;
	bool inview;
};

