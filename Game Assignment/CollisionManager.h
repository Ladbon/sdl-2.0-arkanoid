#pragma once

#include <vector>

struct SDL_Rect;
struct Circle;
class Entity;

namespace CollisionManager {
	bool collideRectPoint(SDL_Rect a, float _point[]);
	bool collideRect(SDL_Rect a, SDL_Rect b);
	bool collideCircle(Circle a, Circle b);
	bool PixelPerfect(Entity* a, Entity* b);
	SDL_Rect NormalizeBounds(SDL_Rect& rect);
	SDL_Rect Intersection(SDL_Rect& a, SDL_Rect& b);
}