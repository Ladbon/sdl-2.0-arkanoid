#include "Text.h"
#include "SDL_ttf.h"
#include "Utils.h"
#include "DrawManager.h"
#include "SDL.h"
#include "Color.h"
#include "Rectangle.h"
#include "Config.h"

Text::Text(int _x, int _y, std::string _fontFamily, int _fontSize, std::string _text, SDL_Renderer* _renderer): Rectangle(_x, _y, 0, 0) {
	fontFamily = _fontFamily;
	fontSize = _fontSize;
	text = _text;
	font = nullptr;
	x = _x;
	y = _y;
	width = 0;
	height = 0;
	renderer = _renderer;
	color = nullptr;
	color = new Color(Config::getInt("default_text_color", 0x000000));

	updateFont();
	updateSize();
	update();
}

Text::~Text() {
	delete color;
	color = nullptr;
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
}

std::string Text::getFontFamily() {
	return fontFamily;
}

std::string Text::getText() {
	return text;
}

int Text::getFontSize() {
	return fontSize;
}

Color Text::getColor() {
	return *color;
}

void Text::setFontFamily(std::string _fontFamily) {
	fontFamily = _fontFamily;
	updateFont();
	updateSize();
}

void Text::setText(std::string _text) {
	text = _text;
	update();
	updateSize();
}

void Text::setFontSize(int _fontSize) {
	fontSize = _fontSize;
	updateFont();
	updateSize();
}

void Text::setColor(int hex) {
	color->setColor(hex);
	update();
}

void Text::draw() {
	SDL_Rect pos = {x, y, width, height};
	SDL_RenderCopy(renderer, texture, nullptr, &pos);
}

void Text::updateFont() {
	std::string filename = "../assets/fonts/" + Utils::toLower(fontFamily) + ".ttf";
	font = TTF_OpenFont(filename.c_str(), fontSize);
	if(!font) printf("Error opening font: %s", TTF_GetError());
}

void Text::update() {
	surface = TTF_RenderText_Blended_Wrapped(font, text.c_str(), color->getAsSDL(), 0);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
}

void Text::updateSize() {
	TTF_SizeUTF8(font, text.c_str(), &width, &height);
}