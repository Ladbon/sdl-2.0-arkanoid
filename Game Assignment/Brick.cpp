#include "Brick.h"
#include "Entity.h"
#include "Config.h"

Brick::Brick(std::string filename, float x, float y, int width, int height): Entity(filename, x, y, width, height) {
}


Brick::~Brick()
{
}

bool Brick::onHit(int dmg) {
	int nextLevel = level - dmg;
	if(nextLevel > 0) {
		level = nextLevel;
		return true;
	}
	return false;
}

int Brick::getScore() {
	return level * Config::getInt("brick_hit_mutliple", 1);
}