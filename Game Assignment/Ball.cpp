#include "Ball.h"
#include "Entity.h"
#include "Utils.h"


Ball::Ball(std::string filename, float x, float y, int width, int height): Entity(filename, x, y, width, height) {
	freezed = false;
	radius = (float)(width * 0.5);
	dx = 0.0f;
	dy = 0.0f;
	speed = 0.0f;
}

Ball::~Ball()
{
}

Circle Ball::getCircle() {
	Circle circle;
	circle.x = x + (float)(width * 0.5);
	circle.y = y + (float)(height * 0.5);
	circle.radius = radius;
	return circle;
}
