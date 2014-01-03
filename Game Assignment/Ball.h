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
	void update(float dt) {
		if(freezed) return;
		x += dx * speed * dt;
		y += dy * speed * dt;
	}

	void move(float _x, float _y) {
		x += _x;
		y += _y;
	}

	float getSpeed() { return speed; }
	float getDirectionX() { return dx; }
	float getDirectionY() { return dy; }
	void invertDirectionX() { dx *= -1; }
	void invertDirectionY() { dy *= -1; }
	
	void freeze() { freezed = true; }
	void unfreeze() { freezed = false; }
	bool isFreezed() { return freezed; }

	Circle getCircle();

private:
	float speed;
	float dx;
	float dy;
	float radius;
	bool freezed;
};

