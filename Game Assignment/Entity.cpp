#include "Entity.h"
#include "SpriteManager.h"
#include "DrawManager.h"
#include "SDL.h"


Entity::Entity(std::string _filename, float _x, float _y, int _width, int _height) {
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	filename = _filename;
	alpha = 255;
}

bool Entity::Create(SpriteManager *spriteManager, int offset_x, int offset_y) {
	sprite = spriteManager->Load(filename, offset_x, offset_y, width, height);
	if(sprite != nullptr) { return false; }
	return true;
}

void Entity::draw(DrawManager *drawManager) {
	drawManager->Draw(this, (int)x - (width * 0.5), (int)y - (height * 0.5));
}


Entity::~Entity()
{
}

SDL_Rect Entity::getBounds() {
	SDL_Rect rect;
	rect.x = (int)x;
	rect.y = (int)y;
	rect.w = width;
	rect.h = height;
	return rect;
}

bool Entity::getAlpha(int x, int y, int sensitivity) {
	int bytes_per_pixel = getSprite()->surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)getSprite()->surface->pixels + y * getSprite()->surface->pitch + x * bytes_per_pixel;
	Uint32 pixelColor;

	switch(bytes_per_pixel) {
	case 1:
		pixelColor = *p;
		break;

	case 2:
		pixelColor = *(Uint16*)p;

	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			pixelColor = p[0] << 16 | p[1] << 8 | p[2];
		} else {
			pixelColor = p[0] | p[1] << 8 | p[2] << 16;
		}

	case 4:
		pixelColor = *(Uint32*)p;
	}

	Uint8 red, green, blue, alpha;
	SDL_GetRGBA(pixelColor, getSprite()->surface->format, &red, &green, &blue, &alpha);
	return alpha > sensitivity;
}

SDL_Rect Entity::NormalizeBounds(SDL_Rect& rect) {
	SDL_Rect normalized;
	normalized.x = rect.x - (int)x;
	normalized.y = rect.y - (int)y;
	normalized.w = rect.w;
	normalized.h = rect.h;
	return normalized;
}