#include "AI.h"
#include "scene.h"

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
	counter++;
	AI::input();

}

void AI::input()
{
	extern Scene scene;
	if (counter % 10 == 0) {
		target = -1;
		distance = 500;
		height = 500;
		int temp;
		for (int i = 0; i < scene.fighter.size(); i++) {
			if (i != character) {
				height = scene.fighter.at(character).pos.y - scene.fighter.at(i).pos.y;
				temp = scene.fighter.at(i).pos.x - scene.fighter.at(character).pos.x;
				if (std::abs(temp) < distance && height<500) {
					distance = temp;
					target = i;
				}
			}
		}
	}
	std::string action = "";
	if (target > -1) {
		if (distance > 100||(distance<0&&distance>=-50 && scene.fighter.at(target).speed.x <= 0)||(scene.fighter.at(character).isFlipped()&&distance>49)) {
			action += "D";
		}
		else if (distance < -100||(distance>=0&&distance<=50&&scene.fighter.at(target).speed.x>=0)||(!scene.fighter.at(character).isFlipped()&&distance<-49)) {
			action += "A";
		}
		else {
			if(std::abs(height)<80) {
				int temp = rand();
				if (temp % 3 == 0)
					action += "SF";
				else if (temp % 3 == 1||scene.fighter.at(character).getState()=="airattack")
					action += "W";
				else
					action += "F";
			}
		}
		if (height > 60) {
			action += "W";
		}
	}
	keys = action;
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