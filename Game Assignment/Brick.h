#pragma once
#include "Entity.h"

class Brick: public Entity {
public:
	Brick(std::string filename, float x, float y, int width, int height);
	~Brick(void);

	void setLevel(int _level) { level = _level; }
	int getLevel() { return level; }

	int getScore();

	bool onHit(int dmg);
private:
	int level;
};

