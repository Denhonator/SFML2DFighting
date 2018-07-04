#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Util.h"
#include <thread>
#include <Windows.h>

Util resources = Util();
Scene scene = Scene();
bool running = true;
float gameSpeed = 60;
int fpslimit = 60;
sf::FloatRect view = sf::FloatRect(0, 0, 2560, 1440);
sf::FloatRect debugview = sf::FloatRect(0, 0, 4000, 2250);

void draw() {
	sf::RenderWindow window(sf::VideoMode(2560, 1440), "FIGHT");
	window.setFramerateLimit(fpslimit);
	window.setVerticalSyncEnabled(false);
	sf::Clock clock;
	sf::Time elapsed;
	std::vector<sf::Sprite> drawlist;
	std::vector<sf::RectangleShape> rectDrawList;
	bool slept = false;
	UINT32 frame = 0;
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
		Util::resetClock();
		if (scene.drawready) {
			window.setView(sf::View(view));
			drawlist = scene.drawlist;
			rectDrawList = scene.rectDrawList;
			scene.drawlist.clear();
			scene.rectDrawList.clear();
			for (int i = 0; i < drawlist.size(); i++)
				window.draw(drawlist.at(i));
			for (int i = 0; i < rectDrawList.size(); i++)
				window.draw(rectDrawList.at(i));
			//_RPT1(0, "Draw took \t%d ", (clock.getElapsedTime() - elapsed).asMicroseconds());
			Util::addTime("Draw");
			sf::Text text = Util::log();
			text.setPosition(sf::Vector2f(view.left, view.top));
			window.draw(text);
			Util::addTime("Debug");
			window.display();
			window.clear();
			scene.drawready = false;
			slept = false;
			//_RPT1(0, "Display took \t%d ", (clock.getElapsedTime() - elapsed).asMicroseconds());
			Util::addTime("Display");
			clock.restart();
		}
		else if (!slept && (16666 * (60.0f / gameSpeed) - elapsed.asMicroseconds() > 10)) {	
			sf::sleep(sf::microseconds(16666.0f * (60.0f / gameSpeed)) - elapsed);
			slept = true;
		}
	}
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
	uint32_t frame = 0;
	sf::Clock clock;
	sf::Time elapsed;
	std::thread drawthread(draw);
	bool slept = false;
	while (running)
	{
		elapsed = clock.getElapsedTime();
		if (elapsed.asMicroseconds()>16666*(60.0f/gameSpeed)) {
			clock.restart();
			frame++;
			Util::resetClock(1);
			scene.update();
			slept = false;
		}
		if(!slept) {
			Util::addTime("Main", 1);
			//_RPT1(0, "\nMain took \t%d ", clock.getElapsedTime().asMicroseconds());
			sf::sleep(sf::microseconds(16667 * (60.0f / gameSpeed)) - (clock.getElapsedTime()));
			slept = true;
		}
	}
	drawthread.join();
	return 0;
}