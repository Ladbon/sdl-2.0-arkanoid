#pragma once
#include "Entity.h"

class Brick: public Entity {
public:
	Brick(std::string filename, float x, float y, int width, int height);
	~Brick(void);
};

