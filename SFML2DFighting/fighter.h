#pragma once
#include "Hitbox.h"
#include "AI.h"
#include "Wall.h"

class Fighter
{
public:
	sf::Vector2f size;
	sf::Vector2f pos;
	sf::Vector2f speed;
	AI ai;
	Fighter(sf::Vector2f boxSize, sf::Vector2f position, sf::String spritesheet, int i, sf::Color color=sf::Color(255,255,255,255));
	Fighter();
	~Fighter();
	sf::Sprite& getSprite();
	sf::Texture* getTexture();
	std::vector<Hitbox> getHitboxes();
	sf::String getState();
	sf::Vector2f getGroundPos();
	float getFrame();
	void getHit(Hitbox hit);
	int getHealth();
	bool isFlipped();
	void physics(std::vector<Wall*> wall, sf::String inputMethod);

private:
	int id;
	bool pressedkey = false;
	sf::Vector2f normalSize;
	sf::Vector2f duckSize;
	sf::Vector2f groundSize;
	sf::Texture* texture;
	sf::Sprite sprite;
	sf::String state;
	int substate;
	sf::String prevState;
	sf::String fighterName;
	bool flip = false;
	float frame;
	float iframe = 0;
	float losecontrol = 0;
	float airtime = 0;
	int health;
	int lives;
	float movementSpeed;
	sf::Vector2f maxSpeed;
	sf::Vector2f prevSpeed;
	sf::Vector2f platformSpeed;
	sf::Vector2f onGroundPos;
	bool canJump = false;
	bool doubleJump = false;
	bool collided = false;
	bool onGround = false;
	int doublejumps = 0;
	float jumpHold = 0;
	std::map <sf::String, sf::IntRect> spriteDefinitions;
	std::vector<Hitbox> hitbox;
	void animate();
	sf::Vector2f collision(std::vector<Wall*> wall);
	sf::String chosenAction(sf::String inputMethod);
	void defineSprites() {
		//std::cout << std::string(fighterName) << std::endl;
		if (fighterName == "toad") {
			spriteDefinitions["idle0"] = sf::IntRect(36, 23, 25, 35);
			spriteDefinitions["idle1"] = sf::IntRect(69, 23, 25, 35);
			spriteDefinitions["duck0"] = sf::IntRect(388, 29, 41, 26);
			spriteDefinitions["run0"] = sf::IntRect(437, 26, 28, 33);
			spriteDefinitions["run1"] = sf::IntRect(467, 26, 25, 33);
			spriteDefinitions["run2"] = sf::IntRect(499, 30, 33, 30);
			spriteDefinitions["run3"] = sf::IntRect(536, 26, 21, 36);
			spriteDefinitions["run4"] = sf::IntRect(561, 30, 33, 30);
			spriteDefinitions["jump0"] = sf::IntRect(499, 30, 33, 30);
			spriteDefinitions["jump1"] = sf::IntRect(561, 30, 33, 30);
			spriteDefinitions["normalattack0"] = sf::IntRect(99, 25, 26, 31);
			spriteDefinitions["normalattack1"] = sf::IntRect(128, 25, 38, 31);
			spriteDefinitions["normalattack2"] = sf::IntRect(36, 23, 25, 35);
			//spriteDefinitions["airattack0"] = sf::IntRect(168, 26, 34, 31);
			spriteDefinitions["airattack0"] = sf::IntRect(209, 17, 34, 41);
			spriteDefinitions["airattack1"] = sf::IntRect(250, 19, 33, 38);
			spriteDefinitions["airattack2"] = sf::IntRect(285, 24, 51, 32);
			spriteDefinitions["groundattack0"] = sf::IntRect(346, 67, 37, 44);
			spriteDefinitions["groundattack1"] = sf::IntRect(391, 70, 66, 41);
			spriteDefinitions["groundattack2"] = sf::IntRect(463, 86, 51, 25);
			spriteDefinitions["fall0"] = sf::IntRect(542, 160, 27, 36);
			spriteDefinitions["fall1"] = sf::IntRect(510, 162, 28, 37);
			spriteDefinitions["fall2"] = sf::IntRect(84, 164, 33, 34);
			spriteDefinitions["fall3"] = sf::IntRect(402, 218, 38, 28);
			spriteDefinitions["fall4"] = sf::IntRect(120, 174, 41, 20);
			//spriteDefinitions["fall5"] = sf::IntRect(447, 127, 33, 27);
			spriteDefinitions["fall5"] = sf::IntRect(549, 131, 38, 21);
		}
	}
};