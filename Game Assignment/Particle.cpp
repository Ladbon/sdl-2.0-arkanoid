#include "Particle.h"
#include "SDL.h"
#include "DrawManager.h"

Particle::Particle(float _x, float _y, float _velx, float _vely, float _speed, int _life, int _color) {
	x = _x;
	y = _y;
	velx = _velx;
	vely = _vely;
	life = _life;
	color = _color;
	speed = _speed;
}

bool Particle::isDead() {
	return (life < 0);
}

void Particle::draw(DrawManager* drawManager) {
	SDL_SetRenderDrawColor(drawManager->getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawPoint(drawManager->getRenderer(), (int)x, (int)y);
}

void Particle::update(float deltatime) {
	life--;
	x += velx * speed * deltatime;
	y += vely * speed * deltatime;
}

float Particle::getX() {
	return x;
}

float Particle::getY() {
	return y;
}

float Particle::getVelX() {
	return velx;
}

float Particle::getVelY() {
	return vely;
}

void Particle::setVelX(float _velx) {
	velx = _velx;
}

void Particle::setVelY(float _vely) {
	vely = _vely;
}