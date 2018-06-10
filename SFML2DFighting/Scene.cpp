#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <iostream> 

Scene::Scene()
{
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(500, 800), "toad.png", 0));
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(1400, 800), "toad.png", 1, sf::Color(255, 50, 50, 255)));

	createWalls();

	if (!background.loadFromFile("background.png"))
		printf("background.png not found");
	bcg.setTexture(background);
	bcg.setScale(5, 5);
	bcg.setPosition(0, -500);
}

Scene::~Scene()
{
	
}

void Scene::createWalls()
{
	wall.push_back(Wall(sf::Vector2f(2000, 10), sf::Vector2f(270, 1200), sf::Color(75, 35, 20)));
	wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(700, 1000), sf::Color(75, 35, 20)));
	wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(1200, 800), sf::Color(75, 35, 20)));
	wall.push_back(Wall(sf::Vector2f(10, 200), sf::Vector2f(1800, 1000), sf::Color(75, 35, 20)));
}

void Scene::update()
{
	for (int i = 0; i < wall.size(); i++) {
		wall.at(i).update();
	}
	if (fighter.at(0).getHealth() <= 0) {
		fighter.at(0) = Fighter(sf::Vector2f(50, 90), sf::Vector2f(500, 800), "toad.png", 0);
	}
	if (fighter.at(1).getHealth() <= 0) {
		fighter.at(1) = Fighter(sf::Vector2f(50, 90), sf::Vector2f(1400, 800), "toad.png", 1, sf::Color(255,50,50,255));
	}
	fighter.at(0).physics(wall, "WASD");
	fighter.at(1).physics(wall, "AI");
	fighter.at(1).ai.play();
	drawlist.push_back(bcg);
	for (int i = 0; i < wall.size(); i++) {
		rectDrawList.push_back(wall.at(i).getRectangle());
	}
	std::vector<Hitbox> hitboxes;
	for (int i = 0; i<fighter.size(); i++) {
		drawlist.push_back(fighter.at(i).getSprite());
		if (false) {
			rectDrawList.push_back(sf::RectangleShape(fighter.at(i).size));
			rectDrawList.back().setPosition(fighter.at(i).pos);
			rectDrawList.back().setFillColor(sf::Color(0, 0, 255, 50));
			rectDrawList.back().setOutlineThickness(1);
		}
		hitboxes = fighter.at(i).getHitboxes();
		for (int j = 0; j<hitboxes.size(); j++) {
			rectDrawList.push_back(hitboxes.at(j).getRectangle());
			std::vector<int> hits = hitboxes.at(j).hitcheck();
			for (int k = 0; k < hits.size(); k++) {
				//std::cout << i << " hit " << hits.at(k) << std::endl;
				fighter.at(hits.at(k)).getHit(hitboxes.at(j));
			}
		}
	}
	drawready = true;
}