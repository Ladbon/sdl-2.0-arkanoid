#include "SDL.h"
#include "Sprite.h"

Sprite::Sprite(SDL_Surface *_surface, SDL_Texture *_texture, int _x, int _y, int _width, int _height) {
	surface = _surface;
	texture = _texture;
	x = _x;
	y = _y;
	width = _width;
	height = _height;
}