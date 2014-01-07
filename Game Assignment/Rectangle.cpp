#include "Rectangle.h"

Rectangle::Rectangle() {

}

Rectangle::Rectangle(float _x, float _y, int _width, int _height) {
	x = _x;
	y = _y;
	width = _width;
	height = _height;
}

Rectangle::~Rectangle() {

}


void Rectangle::setX(float _x) {
	x = _x;
}

void Rectangle::setY(float _y) {
	y = _y;
}

void Rectangle::setWidth(int _width) {
	width = _width;
}

void Rectangle::setHeight(int _height) {
	height = _height;
}

float Rectangle::getX() {
	return x;
}

float Rectangle::getY() {
	return y;
}

int Rectangle::getWidth() {
	return width;
}

int Rectangle::getHeight() {
	return height;
}

int Rectangle::getTop() {
	return y;
}

int Rectangle::getBottom() {
	return y + height;
}

int Rectangle::getLeft() {
	return x;
}

int Rectangle::getRight() {
	return x + width;
}

void Rectangle::move(float _x, float _y) {
	x = _x;
	y = _y;
}

void Rectangle::moveBy(float _x, float _y) {
	x += _x;
	y += _y;
}

void Rectangle::resize(int _width, int _height) {
	width = _width;
	height = _height;
}


bool Rectangle::inside(float _x, float _y) {
	return (x < _x && _x < getRight() && y < _y && _y < getBottom());
}
