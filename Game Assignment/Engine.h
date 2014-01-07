#pragma once
#include <vector>

class State;
class Text;
class DrawManager;
class SpriteManager;
class Entity;
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

	void UpdateTime();
	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return m_running; }
	void Quit() { m_running = false;  };

	DrawManager *getDrawManager() { return drawManager; }
	SpriteManager *getSpriteManager() { return spriteManager; }
private:
	Entity* cursor;
	std::vector<State*> states;
	State *current;

	SDL_Window *window;
	DrawManager *drawManager;
	SpriteManager *spriteManager;

	Text* debug;

	bool m_running;
};