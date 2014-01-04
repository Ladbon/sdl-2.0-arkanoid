#include "Utils.h"
#include "Engine.h"
#include "Config.h"

#include "Menu.h"
#include "Game.h"
#include "Highscore.h"
#include "SDL_ttf.h"

int main(int argc, char *argv[]) {
	if(TTF_Init() == -1) {
		printf("SDL_TTF couldn't be initialized\n");
	}
	Utils::Random::seed();
	Config::parseFile("../config.txt");
	Engine engine;

	engine.Attach(new Menu);
	engine.Attach(new Game);
	engine.Attach(new Highscore);

	if(engine.Init("Arkatris, hybrid name of Arkanoid and Tetris", 0, 0)) {
		engine.SetState("Menu");
		printf("Starting main loop\n");
		while(engine.Running()) {
			engine.HandleEvents();
			engine.Update();
			engine.Draw();
		}
		printf("Ending main loop\n");
	}
	return 0;
}