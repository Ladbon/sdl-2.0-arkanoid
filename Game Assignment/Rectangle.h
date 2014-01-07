#pragma once

class Rectangle {
public:
	Rectangle();
	Rectangle(float _x, float _y, int _width, int _height);
	~Rectangle();

	void setX(float _x);
	void setY(float _y);
	void setWidth(int _width);
	void setHeight(int _height);

	float getX();
	float getY();
	int getWidth();
	int getHeight();

	int getTop();
	int getBottom();
	int getLeft();
	int getRight();

	void move(float _x, float _y);
	void moveBy(float _x, float _y);
	void resize(int _width, int _height);

	bool inside(float _x, float _y);

	float x;
	float y;
	int width;
	int height;
};

