#pragma once

class DrawManager;

class Particle {
	float x;
	float y;
	float velx;
	float vely;
	float speed;
	int life;
	float opacityDegree;
	int color;

public:
	Particle(float _x, float _y, float _velx, float _vely, float _speed, int _life, int _color);
	bool isDead();
	void draw(DrawManager* drawManager);
	void update();
	float getX();
	float getY();
	float getVelX();
	float getVelY();
	void setVelX(float _velx);
	void setVelY(float _vely);
};