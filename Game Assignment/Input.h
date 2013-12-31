#pragma once

enum MouseButtons {
	MB_LEFT,
	MB_RIGHT,
	MB_COUNT
};

/**
*	Keyboard class
*/
class Keyboard {
	friend class Engine;
	friend class State;
public:
	Keyboard();

	bool IsDown(int key) const;
	bool IsDownOnce(int key) const;

	void PostUpdate();

private:
	bool m_current[256];
	bool m_previous[256];
};

/**
*	Mouse class
*/
class Mouse {
	friend class Engine;
	friend class State;
public:
	Mouse();

	int GetX() const;
	int GetY() const;
	bool IsDown(MouseButtons button) const;
	bool IsDownOnce(MouseButtons button) const;

	void PostUpdate();

private:
	bool m_current[MB_COUNT];
	bool m_previous[MB_COUNT];
	int m_x;
	int m_y;
};