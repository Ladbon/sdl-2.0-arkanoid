#pragma once

#include <string>

class DrawManager;
class SpriteManager;
struct SDL_Window;

class State {
	friend class Engine;
public:
	virtual ~State() {};

	virtual bool Init() = 0;
	virtual void Exit() = 0;

	virtual bool Update() = 0;
	virtual void Draw() = 0;
	virtual std::string Next() = 0;
	virtual void setNextState(std::string state) = 0;
	virtual bool IsType(const std::string &type) = 0;

	SDL_Window *window;
	DrawManager *drawManager;
	SpriteManager *spriteManager;

	float deltatime;
	unsigned int ticks;
};