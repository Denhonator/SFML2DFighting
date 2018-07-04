#pragma once
#include <SFML\Graphics.hpp>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h> 

class AI
{
public:
	AI(int id);
	AI();
	~AI();
	sf::String getKeys();
	void play();
	void input();
	bool similar(std::vector<int> int1, std::vector<int> int2);
	bool similar(std::vector<float> float1, std::vector<float> float2);
	bool similar(std::vector<std::string> string1, std::vector<std::string> string2);
	bool run = false;
	int character;

private:
	std::string keys;
	int counter = 0;
	int distance = 500;
	int height = 800;
	std::vector<char> use;
	int target = -1;
};

