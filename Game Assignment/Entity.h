#pragma once

#include "SpriteManager.h"
#include "Sprite.h"
#include <string>

struct SDL_Rect;

class Entity {
public:
	Entity(std::string filename, float x, float y, int width, int height);
	~Entity();

	// Getters
	float getX() { return x; }
	float getY() { return y; }	
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getAlpha() { return alpha; }
	Sprite *getSprite() { return sprite; }
	SDL_Rect getBounds();
	bool getAlpha(int x, int y, int sensitivity);

	// Setters
	void setX(float _x) { x = _x; }
	void setY(float _y) { y = _y; }
	void setWidth(int _width) { width = _width; }
	void setHeight(int _height) { height = _height; }
	void setAlpha(int a) { alpha = a; }

	void move(float _x, float _y) { 
		x += _x; 
		y += _y;
	}
	bool Create(SpriteManager *spriteManager, int offset_x, int offset_y);
	void draw(DrawManager *drawManager);

	std::string toString() { std::string s = "X: " + std::to_string(x) + ", Y: " + std::to_string(y) + ", W: " + std::to_string(width) + ", H: " + std::to_string(height); return s; }

	SDL_Rect NormalizeBounds(SDL_Rect& rect);

	//Returns true if a point (x, y) is withing the rectangle	
	bool collideRectPoint(float _x, float _y);
protected:
	float x;
	float y;
	int width;
	int height;
	std::string filename;
	Sprite *sprite;
	int alpha;
};

