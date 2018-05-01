#include <SFML/Graphics.hpp>
#include "fighter.h"
#include "Wall.h"

void createWalls(Wall wall[]) {
	wall[0] = Wall(sf::Vector2f(2000, 10), sf::Vector2f(270, 1200), sf::Color(100, 100, 255));
	wall[1] = Wall(sf::Vector2f(500, 20), sf::Vector2f(700, 1000), sf::Color(255, 100, 255));
	wall[2] = Wall(sf::Vector2f(500, 20), sf::Vector2f(1200, 800), sf::Color(100, 100, 255));
	wall[3] = Wall(sf::Vector2f(10, 200), sf::Vector2f(1800, 1000), sf::Color(100, 100, 255));
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(2560, 1440), "FIGHT");
	sf::Clock clock;
	sf::Time elapsed;
	int gameSpeed = 60;
	int frameDivider = 1;
	bool skipFrame = false;
	const int wallCount = 20;
	uint32_t frame = 0;

	Fighter blue(sf::Vector2f(50, 90), sf::Vector2f(500, 10), "toad.png");
	Wall wall[wallCount];

	createWalls(wall);

	while (window.isOpen())
	{
		elapsed = clock.getElapsedTime();
		if (elapsed.asMicroseconds()>16666*(60.0f/gameSpeed)) {
			clock.restart();
			skipFrame = false;
			frame++;

			for (int i = 0; i < wallCount; i++) {
				wall[i].update();
			}

			blue.physics(wall);

			if (frame%frameDivider != 0)
				skipFrame = true;
			if (!skipFrame) {
				window.clear();
				window.draw(blue.getSprite());
				for (int i = 0; i < wallCount; i++) {
					window.draw(wall[i].getRectangle());
				}
				window.display();
			}
		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;
}