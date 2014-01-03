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

	void preUpdate() {
		prev_x = x;
		prev_y = y;
	}

	float getPrevX() { return prev_x; }
	float getPrevY() { return prev_y; }
private:
	int lifes;
	float prev_x;
	float prev_y;
};

