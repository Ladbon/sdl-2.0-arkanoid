#pragma once
#include <vector>

class Keyboard;
class Mouse;
class State;
class DrawManager;
class SpriteManager;
struct SDL_Window;

class Engine
{
public:
	Engine();
	~Engine();

	bool Init(const char* title, int width, int height);
	void Cleanup();

	void Attach(State *state);
	void SetState(const std::string &type);
	void ChangeState();

	/**
	*	Handles deltatime and tick specific functions
	*/
	void UpdateTime();

	/**
	*	Handles basic events such as window resize
	*/
	void HandleEvents();


	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false; };

	DrawManager *getDrawManager() { return drawManager; }
	SpriteManager *getSpriteManager() { return spriteManager; }
private:
	std::vector<State*> states;
	State *current;

	SDL_Window *window;
	DrawManager *drawManager;
	SpriteManager *spriteManager;

	Keyboard *keyboard;
	Mouse *mouse;
	
	bool m_running;
};