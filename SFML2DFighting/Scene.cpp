#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Scene::Scene()
{
	srand(time(NULL));
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(500, 400), "toad.png", 0));
	fighter.push_back(Fighter(sf::Vector2f(50, 90), sf::Vector2f(1400, 400), "toad.png", 1, sf::Color(255, 50, 50, 255)));

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
	//wall.push_back(Wall(sf::Vector2f(2000, 10), sf::Vector2f(270, 1200), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));
	/*wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(700, 1000), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));
	wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(1200, 800), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));
	wall.push_back(Wall(sf::Vector2f(500, 20), sf::Vector2f(700, 750), sf::Vector2f(0, 0.2f), sf::Color(255, 255, 255)));
	wall.push_back(Wall(sf::Vector2f(200, 200), sf::Vector2f(1800, 1000), sf::Vector2f(0, 0), sf::Color(255, 255, 255)));*/
	sf::Vector2f pos = sf::Vector2f(270, 1200);
	sf::Vector2f size = sf::Vector2f(2000,10);
	float gridsize = 600;
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			pos.x = i * gridsize + rand() % 201;
			pos.y = j*gridsize + rand() % 701 - 350;
			size.x = rand() % (int)gridsize * 2 + 200;
			size.y = rand() % 11 + 10;
			wall.push_back(Wall(size, pos, sf::Vector2f(0, 0)));
		}
	}
	for (int i = 0; i < wall.size(); i++) {
		wallref.push_back(&wall.at(i));
	}
}

void Scene::update()
{
	//update walls to be updated at interval
	int refresh = 10;
	int refresh1 = refresh + 1;
	int viewbuffer = 100;
	if (frame % refresh1 == 0) {	
		toUpdate = toUpdateBuf;
		toUpdateBuf.clear();
	}
	//Decide on walls to be updated in equal slices each frame
	extern sf::FloatRect view;
	for (int i = (frame%refresh1)*wall.size()/refresh; i < std::min(((frame+1)%refresh1)*wall.size() / refresh, wall.size()); i++) {
		if (wall.at(i).inRect(sf::FloatRect(view.left - viewbuffer, view.top - viewbuffer, view.width + viewbuffer * 2, view.height + viewbuffer * 2)) || wall.at(i).global) {
			toUpdateBuf.push_back(i);
		}
	}
	//update walls in view
	for (int i = 0; i < toUpdate.size(); i++) {
		wall.at(toUpdate.at(i)).update();
	}
	Util::addTime("Wall update", 1);

	if (fighter.at(0).getHealth() <= 0) {
		fighter.at(0) = Fighter(sf::Vector2f(50, 90), fighter.at(0).getGroundPos(), "toad.png", 0);
	}
	if (fighter.at(1).getHealth() <= 0) {
		fighter.at(1) = Fighter(sf::Vector2f(50, 90), fighter.at(1).getGroundPos(), "toad.png", 1, sf::Color(255,50,50,255));
	}
	fighter.at(0).physics(wallref, "WASD");
	fighter.at(1).physics(wallref, "AI");
	fighter.at(1).ai.play();

	if(!drawready) {
		drawlist.push_back(bcg);
		for (int i = 0; i < toUpdate.size(); i++) {
			drawlist.push_back(wallref.at(toUpdate.at(i))->getSprite());
		}
		std::vector<Hitbox> hitboxes;
		sf::Vector2f middle = sf::Vector2f(-view.width/2, -view.height/2);
		for (int i = 0; i < fighter.size(); i++) {
			drawlist.push_back(fighter.at(i).getSprite());
			middle += fighter.at(i).pos / (float)fighter.size();
			/*rectDrawList.push_back(sf::RectangleShape(fighter.at(i).size));
			rectDrawList.back().setPosition(fighter.at(i).pos);
			rectDrawList.back().setFillColor(sf::Color(0, 0, 255, 50));
			rectDrawList.back().setOutlineThickness(1);*/
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
		view = sf::FloatRect(middle, sf::Vector2f(view.width, view.height));
		drawready = true;
	}
	frame++;
}