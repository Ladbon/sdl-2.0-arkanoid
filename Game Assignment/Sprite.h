#pragma once

struct SDL_Texture;
struct SDL_Surface;

class Sprite {
	friend class DrawManager;

public:
	Sprite(SDL_Surface *surface, SDL_Texture *texture, int x, int y, int width, int height);
	SDL_Surface *surface;

protected:
	SDL_Texture *texture;

	int x;
	int y;
	int width;
	int height;
};