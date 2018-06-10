#include "AI.h"
#include "scene.h"
#include <iostream>

AI::AI(int id)
{
	srand(time(NULL));
	keys = "";
	character = id;
	use = { 'A', 'D', 'W', 'S', 'F' };
}

AI::AI()
{
}

AI::~AI()
{
}

sf::String AI::getKeys()
{
	return keys;
}

void AI::play()
{
	if (!run)
		return;
	fbuffer.clear();
	ibuffer.clear();
	sbuffer.clear();

	std::vector<int> prevhealth = health;
	health.clear();

	extern Scene scene;

	for (int i = 0; i < scene.fighter.size(); i++) {
		fbuffer.push_back(scene.fighter.at(i).pos.x);
		fbuffer.push_back(scene.fighter.at(i).pos.y);
		fbuffer.push_back(scene.fighter.at(i).speed.x);
		fbuffer.push_back(scene.fighter.at(i).speed.y);

		ibuffer.push_back(scene.fighter.at(i).getFrame());
		ibuffer.push_back(scene.fighter.at(i).getHealth());

		sbuffer.push_back(scene.fighter.at(i).getState());

		health.push_back(scene.fighter.at(i).getHealth());
	}
	AI::input();

	floats.push_back(fbuffer);
	ints.push_back(ibuffer);
	strings.push_back(sbuffer);

	if (!prevhealth.empty()) {
		for (int i = 0; i < scene.fighter.size(); i++) {
			if (health.at(i) != prevhealth.at(i)) {
				if (i == character) {
					badfloats.reserve(badfloats.size() + floats.size());
					badfloats.insert(badfloats.end(), floats.begin(), floats.end());
					badints.reserve(badints.size() + ints.size());
					badints.insert(badints.end(), ints.begin(), ints.end());
					badstrings.reserve(badstrings.size() + strings.size());
					badstrings.insert(badstrings.end(), strings.begin(), strings.end());
				}
				else {
					goodfloats.reserve(goodfloats.size() + floats.size());
					goodfloats.insert(goodfloats.end(), floats.begin(), floats.end());
					goodints.reserve(goodints.size() + ints.size());
					goodints.insert(goodints.end(), ints.begin(), ints.end());
					goodstrings.reserve(goodstrings.size() + strings.size());
					goodstrings.insert(goodstrings.end(), strings.begin(), strings.end());
				}
				floats.clear();
				ints.clear();
				strings.clear();
			}
		}
	}
}

void AI::input()
{
	int threshold = 15;
	int randmax = 1000;
	std::map<char, int> chance;
	for (int i = 0; i < use.size(); i++)
		chance[use.at(i)] = threshold;
	for (int i = 0; i < goodfloats.size(); i++) {
		if (AI::similar(fbuffer, goodfloats.at(i))) {
			for (int j = 0; j < use.size(); j++) {
				if (sbuffer.back().find(use.at(j))) {
					chance[use.at(j)] += (randmax - chance[use.at(j)]) / 4;
					std::cout << "Chance increased for " << use.at(j) << "\n";
				}
			}	
		}
	}
	sf::String action = "";
	if (rand() % randmax + 1 <= chance['A'])
		action += "A";
	else if (rand() % randmax + 1 <= chance['D'])	//can't move in both directions at once
		action += "D";
	if (rand() % randmax + 1 <= chance['W'])
		action += "W";
	if (rand() % randmax + 1 <= chance['S'])
		action += "S";
	if (rand() % randmax + 1 <= chance['F'])
		action += "F";
	keys = action;
	sbuffer.push_back(keys);
}

bool AI::similar(std::vector<int> int1, std::vector<int> int2) {
	int leeway = 1;
	for (int i = 0; i < int1.size(); i++) {
		if (std::abs(int1.at(i) - int2.at(i)) > leeway)
			return false;
	}
	return true;
}
bool AI::similar(std::vector<float> float1, std::vector<float> float2) {
	float leeway = 1;
	for (int i = 0; i < float1.size(); i++) {
		if (std::abs(float1.at(i) - float2.at(i)) > leeway)
			return false;
	}
	return true;
}
bool AI::similar(std::vector<sf::String> string1, std::vector<sf::String> string2) {
	for (int i = 0; i < string1.size(); i++) {
		if (string1.at(i) != string2.at(i))
			return false;
	}
	return true;
}