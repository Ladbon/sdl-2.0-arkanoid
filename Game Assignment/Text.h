#pragma once

#include <string>
#include "Rectangle.h"

class DrawManager;
struct SDL_Renderer;
class Color;
struct SDL_Texture;
struct SDL_Surface;
typedef struct _TTF_Font TTF_Font;

class Text : public Rectangle {
public:
	Text(int _x, int _y, std::string fontFamily, int fontSize, std::string text, SDL_Renderer* renderer);
	~Text();

	std::string getFontFamily();
	std::string getText();
	int getFontSize();
	Color getColor();

	void setFontFamily(std::string fontFamily);
	void setText(std::string text);
	void setFontSize(int fontSize);
	void setColor(int hex);

	void draw();

	void update();
	void updateSize();
	void updateFont();

private:
	std::string fontFamily;
	std::string text;
	int fontSize;
	Color* color;
	TTF_Font* font;
	SDL_Surface* surface;
	SDL_Texture* texture;
	SDL_Renderer* renderer; // Save renderer so that we can apply changes like color and text without needing to pass the renderer
};