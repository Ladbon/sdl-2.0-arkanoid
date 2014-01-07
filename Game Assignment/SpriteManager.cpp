#include "SDL_image.h"
#include "SpriteManager.h"
#include "DrawManager.h"
#include "Sprite.h"

SpriteManager::SpriteManager(DrawManager *_draw_manager)
{
	draw_manager = _draw_manager;
}


SpriteManager::~SpriteManager()
{
}

bool SpriteManager::Init(const std::string &_directory) {
	int flags = IMG_INIT_PNG;
	int initialized = IMG_Init(flags);

	if((initialized & flags) != flags) {
		printf("[IMG] IMG_Init error: %s\n", IMG_GetError());
		return false;
	}
	directory = _directory;

	return true;
}

void SpriteManager::Cleanup() {
	std::map<std::string, Pair>::iterator it = sprites.begin();
	while(it != sprites.end()) {
		SDL_FreeSurface(it->second.surface);
		SDL_DestroyTexture(it->second.texture);
		++it;
	}

	sprites.clear();

	IMG_Quit();
}

Sprite *SpriteManager::Load(const std::string &filename, int x, int y, int width, int height) {
	std::map<std::string, Pair>::iterator it = sprites.find(filename);
	if(it == sprites.end()) { 
		std::string path = directory + filename;
		
		SDL_Surface *surface = IMG_Load(path.c_str());
		if(surface == nullptr) {
			return nullptr;
		}
		
		SDL_Texture *texture = SDL_CreateTextureFromSurface(draw_manager->renderer, surface);
		
		Pair pair = { surface, texture };

		sprites.insert(std::pair<std::string, Pair>(filename, pair));
		it = sprites.find(filename);
	}
	return new Sprite(it->second.surface, it->second.texture, x, y, width, height);
}
