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

	int getScore() { return score; }
	void setScore(int _score) { score = _score; }
	void appendScore(int _append) { score += _append; }

private:
	int lifes;
	int score;
};

