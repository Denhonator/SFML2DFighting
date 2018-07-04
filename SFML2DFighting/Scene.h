#pragma once
#include <SFML/Graphics.hpp>
#include "Fighter.h"
#include "Wall.h"
#include "Util.h"
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
	std::vector<int> toUpdateBuf;
	std::vector<int> toUpdate;
	std::vector<sf::Sprite> drawlist;
	std::vector<sf::RectangleShape> rectDrawList;
	bool drawready = false;
private:
	uint32_t frame = 0;
};

