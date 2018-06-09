#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Fighter.h"
#include "Wall.h"

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
	Scene scene = Scene();

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
				/*window.clear();
				if (!scene.drawlist.empty()) {
					for (int i = 0; i < scene.drawlist.size(); i++) {
						window.draw(*scene.drawlist.at(i));
					}
				}
				scene.drawlist.clear();
				if (!scene.rectDrawList.empty()) {
					for (int i = 0; i < scene.rectDrawList.size(); i++)
						window.draw(*scene.rectDrawList.at(i));
				}
				scene.rectDrawList.clear();
				window.display();*/
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