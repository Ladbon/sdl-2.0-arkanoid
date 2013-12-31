#pragma once
#include "Entity.h"

struct Circle;

class Ball: public Entity {
public:
	Ball(std::string filename, float x, float y, int width, int height);
	~Ball(void);

	void setSpeed(float _speed) { speed = _speed; }
	void setDirection(float _dx, float _dy) { dx = _dx; dy = _dy; }
	void setDirectionX(float _dx) { dx = _dx; }
	void setDirectionY(float _dy) { dy = _dy; }
	void move(float dt) {
		if(freezed) return;
		x += dx * speed * dt;
		y += dy * speed * dt;
	}

	float getSpeed() { return speed; }
	float getDirectionX() { return dx; }
	float getDirectionY() { return dy; }
	
	void freeze() { freezed = true; }
	void unfreeze() { freezed = false; }

	Circle getCircle();

private:
	float speed;
	float dx;
	float dy;
	float radius;
	bool freezed;
};

