#include "Utils.h"
#include "Engine.h"
#include "Config.h"

#include "Menu.h"
#include "Options.h"
#include "GFXState.h"
#include "Game.h"
#include "Highscore.h"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "Sound.h"

int main(int argc, char *argv[]) {	
	Config::parseFile("../config.txt");

	if(TTF_Init() == -1) {
		printf("SDL_TTF couldn't be initialized\n");
	}

	int flags = MIX_INIT_OGG | MIX_INIT_MP3;
	if(((Mix_Init(flags) & flags) != flags) || (-1 == Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024))) {
		printf("Couldn't initialize audio: %s\n", Mix_GetError());
	}
	Mix_AllocateChannels(16);

	// Init sounds
	SoundManager::SoundManager();
	SoundManager::setSoundDir(Config::get("sound_path"));
	SoundManager::setMusicDir(Config::get("music_path"));
	SoundManager::addSound("paddle_bounce", "paddle_bounce.wav");
	SoundManager::addSound("block_hit", "block_hit.wav");
	SoundManager::addSound("block_destroy", "block_destroy.wav");
	SoundManager::addSound("menu_select", "menu_select.wav");
	SoundManager::addMusic("only_time", "only_time.mp3");

	Utils::Random::seed();
	Engine engine;
	engine.Attach(new Menu);
	engine.Attach(new Options);
	engine.Attach(new GFXState);
	engine.Attach(new Game);
	engine.Attach(new Highscore);
	if(engine.Init(Config::get("name").c_str(), 0, 0)) {
		engine.SetState("Menu");
		while(engine.Running()) {
			engine.HandleEvents();
			engine.Update();
			engine.Draw();
		}
	}
	Mix_AllocateChannels(0);
	return 0;
}