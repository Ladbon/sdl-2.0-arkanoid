#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	Player(std::string filename, float x, float y, int width, int height);
	~Player();

	int getLifes() { return lifes; }
	void decreaseLife() { lifes--; if(lifes < 0) lifes = 0; }
	void increaseLife() { lifes++; }
	void setLifes(int _lifes) { lifes = _lifes; }
private:
	int lifes;
};

