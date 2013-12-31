#define Maximum(a, b) ((a > b) ? a : b)
#define Minimum(a, b) ((a < b) ? a : b)

#include "CollisionManager.h"
#include "Entity.h"
#include "SDL.h" // For SDL_Rect
#include "Utils.h" // For SDL_Circle

namespace CollisionManager {
	/* Rectangle vs Point intersection */
	bool collideRectPoint(SDL_Rect a, float _point[]) {
		return (a.x >= _point[0] && _point[0] <= (a.x + a.w) && a.y >= _point[1] && _point[1] <= (a.y + a.h));
	}

	/* Rectangle vs Rectangle intersection */
	bool collideRect(SDL_Rect a, SDL_Rect b) {
		return !(a.x + a.w < b.x || a.y + a.h < b.y || a.x > b.x + b.w || a.y > b.y + b.h);
	}

	std::vector<float> collideRectPlus(SDL_Rect a, SDL_Rect b) {
		std::vector<float> r(2, 0);
		float length_x = fabs((float)(a.x - b.x));
		float length_y = fabs((float)(a.y - b.y));
		if(length_x < (a.w * 0.5 + b.w * 0.5) && length_y < (a.h * 0.5 + b.h * 0.5)) {
			float overflow_x = length_x - (a.w * 0.5 + b.w * 0.5);
			float overflow_y = length_y - (a.h * 0.5 + b.h * 0.5);
			if(Utils::AreSame(overflow_x, overflow_y)) overflow_x++;

			if(overflow_x > overflow_y) {
				if(a.x > b.x) {
					r[0] = -overflow_x;
				} else {
					r[0] = overflow_x;
				}
			} else {
				if(a.y > b.y) {
					r[1] = -overflow_y;
				} else {
					r[1] = overflow_y;
				}
			}
		}
		return r;
	}

	/* Circle vs Circle intersection */
	bool collideCircle(Circle a, Circle b) {
		float dx = b.x - a.x;
		float dy = b.y - a.y;
		if(a.radius + b.radius > sqrtf(dx * dx + dy * dy)) {
			return true;
		}
		return false;
	}

	SDL_Rect Intersection(const SDL_Rect& a, const SDL_Rect& b) {
		int x1 = Maximum(a.x, b.x);
		int y1 = Maximum(a.y, b.y);
		int x2 = Minimum(a.x + a.w, b.x + b.w);
		int y2 = Minimum(a.y + a.h, b.y + b.h);

		int width = x2 - x1;
		int height = y2 - y1;

		if(width > 0 && height > 0) {
			SDL_Rect intersect = {x1, y1, width, height};
			return intersect;
		} else {
			SDL_Rect intersect = {0, 0, 0, 0};
			return intersect;
		}
	}

	bool PixelPerfect(Entity* a, Entity* b) {
		SDL_Rect collisionRect = Intersection(a->getBounds(), b->getBounds());

		if(collisionRect.w == 0 && collisionRect.h == 0) return false;

		SDL_Rect normalA = a->NormalizeBounds(collisionRect);
		SDL_Rect normalB = b->NormalizeBounds(collisionRect);

		for(int y = 0; y < collisionRect.h; y++) {
			for(int x = 0; x < collisionRect.w; x++) {
				if(a->getAlpha(normalA.x + x, normalA.y + y, 200) && b->getAlpha(normalB.x + x, normalB.y + y, 200)) {
					return true;
				}
			}
		}

		return false;
	}
}