#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	Player(std::string filename, float x, float y, int width, int height);
	~Player();
};

