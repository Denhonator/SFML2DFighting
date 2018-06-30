#include "AI.h"
#include "scene.h"

AI::AI(int id)
{
	srand(time(NULL));
	keys = "";
	character = id;
	use = { 'A', 'D', 'W', 'S', 'F' };
	fbuffer.reserve(480);
	sbuffer.reserve(480);
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

	if (loop >= 60)
		resizing = true;
	else {
		loop++;
		resizing = false;
	}

	std::vector<int> prevhealth = health;
	health.clear();

	extern Scene scene;

	if (resizing) {
		fbuffer.erase(fbuffer.begin(), fbuffer.begin()+floats);
		//std::cout << fbuffer.size() << "\n";

		sbuffer.erase(sbuffer.begin(), sbuffer.begin() + strings);
		//std::cout << sbuffer.size() << "\n";

		keysbuffer.erase(keysbuffer.begin());
		//std::cout << keysbuffer.size() << "\n";
	}

	for (int i = 0; i < scene.fighter.size(); i++) {
		fbuffer.push_back(scene.fighter.at(i).pos.x);
		fbuffer.push_back(scene.fighter.at(i).pos.y);

		sbuffer.push_back(scene.fighter.at(i).getState());

		health.push_back(scene.fighter.at(i).getHealth());
	}
	AI::input();

	if (!prevhealth.empty()) {
		for (int i = 0; i < scene.fighter.size(); i++) {
			if (health.at(i) != prevhealth.at(i)) {
				if (i == character) {	//take damage
					badf.reserve(badf.size() + fbuffer.size());
					badf.insert(badf.end(), fbuffer.begin(), fbuffer.end());
					bads.reserve(bads.size() + sbuffer.size());
					bads.insert(bads.end(), sbuffer.begin(), sbuffer.end());
					badkeys.reserve(badkeys.size() + keysbuffer.size());
					badkeys.insert(badkeys.end(), keysbuffer.begin(), keysbuffer.end());
				}
				else {					//deal damage
					goodf.reserve(goodf.size() + fbuffer.size());
					goodf.insert(goodf.end(), fbuffer.begin(), fbuffer.end());
					goods.reserve(goods.size() + sbuffer.size());
					goods.insert(goods.end(), sbuffer.begin(), sbuffer.end());
					goodkeys.reserve(goodkeys.size() + keysbuffer.size());
					goodkeys.insert(goodkeys.end(), keysbuffer.begin(), keysbuffer.end());
				}
			}
		}
	}
}

void AI::input()
{
	int threshold = 15;
	int randmax = 1000;
	float similarity = 2.0f;
	std::map<char, int> chance;
	for (int i = 0; i < use.size(); i++)
		chance[use.at(i)] = threshold;
	for (int i = 0; i < goodf.size(); i += floats) {
		for (int j = 0; j < floats; j++) {
			if (std::abs(fbuffer.at(fbuffer.size()-(floats-j)) - goodf.at(i + j)) > similarity)
				break;
			else if (j == floats-1) {	//On finding a matching scenario for floats
				for (int k = 0; k < use.size(); k++) {
					if (goodkeys.at((i / floats)).find(use.at(k)) != std::string::npos) {
						chance[use.at(k)] += (randmax - chance[use.at(k)]) * 2;
					}
				}
			}
		}
	}
	std::string action = "";
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
	keysbuffer.push_back(keys);
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
bool AI::similar(std::vector<std::string> string1, std::vector<std::string> string2) {
	for (int i = 0; i < string1.size(); i++) {
		if (string1.at(i) != string2.at(i))
			return false;
	}
	return true;
}