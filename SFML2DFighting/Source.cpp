#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Fighter.h"
#include "Wall.h"

Scene scene = Scene();
int main()
{
	sf::RenderWindow window(sf::VideoMode(2560, 1440), "FIGHT");
	sf::Clock clock;
	sf::Time elapsed;
	int gameSpeed = 60;
	int frameDivider = 1;
	bool skipFrame = false;
	uint32_t frame = 0;

	while (window.isOpen())
	{
		elapsed = clock.getElapsedTime();
		if (elapsed.asMicroseconds()>16666*(60.0f/gameSpeed)) {
			clock.restart();
			skipFrame = false;
			frame++;

			scene.update();

			if (frame%frameDivider != 0)
				skipFrame = true;
			if (!skipFrame) {
				scene.draw(&window);
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