#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Fighter.h"
#include "Wall.h"
#include <thread>

Scene scene = Scene();
bool running = true;
int gameSpeed = 60;
sf::FloatRect view = sf::FloatRect(0, 0, 2560, 1440);

void draw() {
	sf::RenderWindow window(sf::VideoMode(2560, 1440), "FIGHT");
	window.setView(sf::View(view));
	sf::Clock clock;
	sf::Time elapsed;
	std::vector<sf::Sprite> drawlist;
	std::vector<sf::RectangleShape> rectDrawList;
	bool slept = false;
	while (running&&window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				running = false;
				window.close();
			}
		}
		elapsed = clock.getElapsedTime();
		if (scene.drawready) {
			drawlist = scene.drawlist;
			rectDrawList = scene.rectDrawList;
			scene.drawready = false;
			clock.restart();
			scene.drawlist.clear();
			scene.rectDrawList.clear();
			window.clear();

			for (int i = 0; i < drawlist.size(); i++)
				window.draw(drawlist.at(i));

			for (int i = 0; i < rectDrawList.size(); i++)
				window.draw(rectDrawList.at(i));

			window.display();
			slept = false;
		}
		else if (!slept && (16666 * (60.0f / gameSpeed) - elapsed.asMicroseconds() > 10)) {
			//std::cout << "Draw sleeping for " << (sf::microseconds(16667 * (60.0f / gameSpeed)) - elapsed).asMicroseconds() << "\n";
			sf::sleep(sf::microseconds(16666 * (60.0f / gameSpeed)) - elapsed);
			slept = true;
		}
	}
}

int main()
{
	uint32_t frame = 0;
	sf::Clock clock;
	sf::Time elapsed;
	std::thread drawthread(draw);
	while (running)
	{
		elapsed = clock.getElapsedTime();
		if (elapsed.asMicroseconds()>16666*(60.0f/gameSpeed)) {
			clock.restart();
			frame++;

			scene.update();
		}
		else {
			//std::cout << "Main sleeping for " << (sf::microseconds(16667 * (60.0f / gameSpeed)) - elapsed).asMicroseconds() << "\n";
			sf::sleep(sf::microseconds(16667 * (60.0f / gameSpeed)) - elapsed);
		}
	}
	drawthread.join();
	return 0;
}