#include "DrawManager.h"
#include "Entity.h"
#include "SDL.h"
#include "Utils.h"
#include "Config.h"
#include "Sprite.h"
#include <stdio.h>

DrawManager::~DrawManager()
{
}

bool DrawManager::Initialize(SDL_Window *window, int _width, int _height) {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, _width, _height);
	if(renderer == nullptr) {
		return false;
	};

	width = _width;
	height = _height;

	return true;
};

void DrawManager::Cleanup() {
	if(renderer != nullptr) {
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
	}
}

void DrawManager::Clear() {
	SDL_SetRenderDrawColor(renderer, Utils::string_to_int(Config::get("background_red")), Utils::string_to_int(Config::get("background_green")), Utils::string_to_int(Config::get("background_blue")), Utils::string_to_int(Config::get("background_alpha")));
	SDL_RenderClear(renderer);
}

void DrawManager::Present() {
	SDL_RenderPresent(renderer);
}

void DrawManager::Draw(Entity *entity, int x, int y) {
	SDL_Rect src = { entity->getSprite()->x, entity->getSprite()->y, entity->getSprite()->width, entity->getSprite()->height };
	SDL_Rect dst = { x, y, entity->getSprite()->width, entity->getSprite()->height };
	SDL_SetTextureAlphaMod(entity->getSprite()->texture, entity->getAlpha());
	SDL_RenderCopy(renderer, entity->getSprite()->texture, &src, &dst);
}

