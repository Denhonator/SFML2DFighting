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
	std::vector<float> fbuffer;
	std::vector<float> goodf;
	std::vector<float> badf;
	std::vector<std::string> sbuffer;
	std::vector<std::string> goods;
	std::vector<std::string> bads;
	std::vector<int> health;

	std::vector<std::string> keysbuffer;
	std::vector<std::string> goodkeys;
	std::vector<std::string> badkeys;
	std::string keys;

	std::vector<char> use;

	int loop = 0;
	int floats = 4;
	int strings = 2;
	bool resizing;
};

