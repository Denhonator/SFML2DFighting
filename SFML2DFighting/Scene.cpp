#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <iostream> 

Scene::Scene()
{
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(500, 800), "toad.png"));
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(600, 800), "toad.png"));

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
	fighter.at(0).physics(wall, "WASD");
	fighter.at(1).physics(wall, "ARROWS");
	drawlist.push_back(bcg);
	for (int i = 0; i < wall.size(); i++) {
		rectDrawList.push_back(wall.at(i).getRectangle());
	}
	std::vector<Hitbox> hitboxes;
	for (int i = 0; i<fighter.size(); i++) {
		drawlist.push_back(fighter.at(i).getSprite());
		hitboxes = fighter.at(i).getHitboxes();
		for (int j = 0; j<hitboxes.size(); j++) {
			rectDrawList.push_back(hitboxes.at(j).getRectangle());
		}
	}
}

void Scene::draw(sf::RenderWindow *window)
{
	window->clear();
	if (!drawlist.empty()) {
		for (int i = 0; i < drawlist.size(); i++) {
			window->draw(drawlist.at(i));
		}
	}
	drawlist.clear();
	if (!rectDrawList.empty()) {
		for (int i = 0; i < rectDrawList.size(); i++)
			window->draw(rectDrawList.at(i));
	}
	rectDrawList.clear();
	window->display();
}