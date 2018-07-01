#pragma once
#include <SFML/Graphics.hpp>
#include "Fighter.h"
#include "Wall.h"
class Scene
{
public:
	std::vector<Fighter> fighter;
	std::vector<Wall> wall;
	std::vector<Wall*> wallref;
	sf::Texture background;
	sf::Sprite bcg;
	Scene();
	~Scene();
	void createWalls();
	void update();
	std::vector<sf::Sprite> drawlist;
	std::vector<sf::RectangleShape> rectDrawList;
	bool drawready = false;
};

