#include "Player.h"
#include "Entity.h"
#include "Config.h"
#include "Utils.h"

Player::Player(std::string filename, float x, float y, int width, int height): Entity(filename, x, y, width, height) {
	lifes = Config::getInt("start_lifes", 3);
	score = 0;
}


Player::~Player()
{
}
