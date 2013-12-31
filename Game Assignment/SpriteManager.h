#pragma once

#include <map>
#include <string>

class DrawManager;
class Sprite;

class SpriteManager {
	struct Pair {
		struct SDL_Surface *surface;
		struct SDL_Texture *texture;
	};

public:
	SpriteManager(DrawManager *_draw_manager);
	~SpriteManager();

	/**
	*	Initialize SDL Image module. Must only be called once
	*/
	bool Init(const std::string &_directory);

	/**
	*	Frees memory be deallocating all SDL_Surfaces and al SDL_Textures
	*/
	void Cleanup();

	/**
	*	Loads a file if it has not already been loaded and returns a Sprite object with a SDL_Surface
	*	and SDL_Texture and also dimensions (x, y, w, h)
	*/
	Sprite *Load(const std::string &filename, int x, int y, int width, int height);

private:
	DrawManager *draw_manager;
	std::string directory;
	std::map<std::string, Pair> sprites;
};

