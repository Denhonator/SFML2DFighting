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
	bool similar(std::vector<sf::String> string1, std::vector<sf::String> string2);
	bool run = false;
	int character;

private:
	std::vector<float> fbuffer;
	std::vector<std::vector<float>> floats;
	std::vector<std::vector<float>> goodfloats;
	std::vector<std::vector<float>> badfloats;

	std::vector<int> ibuffer;
	std::vector<std::vector<int>> ints;
	std::vector<std::vector<int>> goodints;
	std::vector<std::vector<int>> badints;

	std::vector<sf::String> sbuffer;
	std::vector<std::vector<sf::String>> strings;
	std::vector<std::vector<sf::String>> goodstrings;
	std::vector<std::vector<sf::String>> badstrings;

	std::vector<int> health;

	sf::String keys;

	std::vector<char> use;
};

