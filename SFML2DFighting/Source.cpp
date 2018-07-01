#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Debug.h"
#include <thread>
#include <Windows.h>

Scene scene = Scene();
Debug debug = Debug();
bool running = true;
float gameSpeed = 60;
int fpslimit = 60;
sf::FloatRect view = sf::FloatRect(0, 0, 2560, 1440);

void draw() {
	sf::RenderWindow window(sf::VideoMode(2560, 1440), "FIGHT");
	window.setFramerateLimit(fpslimit);
	window.setVerticalSyncEnabled(false);
	window.setView(sf::View(view));
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
		if (scene.drawready) {
			drawlist = scene.drawlist;
			rectDrawList = scene.rectDrawList;
			scene.drawlist.clear();
			scene.rectDrawList.clear();
			for (int i = 0; i < drawlist.size(); i++)
				window.draw(drawlist.at(i));
			for (int i = 0; i < rectDrawList.size(); i++)
				window.draw(rectDrawList.at(i));
			debug.add("Draw took " + std::to_string((clock.getElapsedTime() - elapsed).asMicroseconds()));
			window.draw(debug.log());
			debug.add("Debug took " + std::to_string((clock.getElapsedTime() - elapsed).asMicroseconds()));
			window.display();
			window.clear();
			scene.drawready = false;
			slept = false;
			debug.add("Display took " + std::to_string((clock.getElapsedTime() - elapsed).asMicroseconds()));
			clock.restart();
		}
		else if (!slept && (16666 * (60.0f / gameSpeed) - elapsed.asMicroseconds() > 10)) {			
			//debuglog(std::to_string((sf::microseconds(16667 * (60.0f / gameSpeed)) - elapsed).asMicroseconds()) + "\n" + text.getString());
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

			scene.update();
			slept = false;
		}
		if(!slept) {
			debug.add("Main took " + std::to_string((clock.getElapsedTime()).asMicroseconds()));
			sf::sleep(sf::microseconds(16667 * (60.0f / gameSpeed)) - (clock.getElapsedTime()));
			slept = true;
		}
	}
	drawthread.join();
	return 0;
}