#include "Scene.h"
#include <SFML/Graphics.hpp>

Scene::Scene()
{
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(500, 800), "toad.png", 0));
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(1400, 800), "toad.png", 1, sf::Color(255, 50, 50, 255)));

	createWalls();

	if (!background.loadFromFile("background.png"))
		return;
	bcg.setTexture(background);
	bcg.setScale(5, 5);
	bcg.setPosition(0, -500);
}

Scene::~Scene()
{
	
}

void Scene::createWalls()
{
	wall.push_back(Wall(sf::Vector2f(2000, 10), sf::Vector2f(270, 1200), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));
	wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(700, 1000), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));
	wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(1200, 800), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));
	wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(700, 750), sf::Vector2f(0, 0.2f), sf::Color(255, 255, 255)));
	wall.push_back(Wall(sf::Vector2f(200, 200), sf::Vector2f(1800, 1000), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));
	for (int i = 0; i < wall.size(); i++) {
		wallref.push_back(&wall.at(i));
	}
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
	fighter.at(0).physics(wallref, "WASD");
	fighter.at(1).physics(wallref, "ARROWS");
	fighter.at(1).ai.play();
	if(!drawready) {
		drawlist.push_back(bcg);
		for (int i = 0; i < wall.size(); i++) {
			drawlist.push_back(wall.at(i).getSprite());
		}
		std::vector<Hitbox> hitboxes;
		for (int i = 0; i < fighter.size(); i++) {
			drawlist.push_back(fighter.at(i).getSprite());
			if (false) {
				rectDrawList.push_back(sf::RectangleShape(fighter.at(i).size));
				rectDrawList.back().setPosition(fighter.at(i).pos);
				rectDrawList.back().setFillColor(sf::Color(0, 0, 255, 50));
				rectDrawList.back().setOutlineThickness(1);
			}
			hitboxes = fighter.at(i).getHitboxes();
			for (int j = 0; j < hitboxes.size(); j++) {
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
}