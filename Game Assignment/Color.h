#pragma once

#include <stdint.h>
#include <string>

struct SDL_Color;

class Color
{
public:
	Color();
	Color(int color_mask);
	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a=255);
	Color(std::string colorName);

	Color operator+(const Color& color) const;
	Color operator-(const Color& color) const;
	bool  operator==(const Color& color) const;
	bool  operator!=(const Color& color) const;

	uint8_t r();
	uint8_t g();
	uint8_t b();
	uint8_t a();

	void randomize();
	void randomizeAlpha();
	void setColor(int color_mask);

	SDL_Color getAsSDL();

	void invert(bool R=true, bool G=true, bool B=true);
private:
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t alpha;
};