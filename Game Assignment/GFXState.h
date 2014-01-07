#pragma once

#include <vector>

#include "State.h"

class Entity;
class Text;

class GFXState: public State {
public:
	GFXState();
	bool Init();
	void Exit();

	bool Update();
	void Draw();

	std::string Next();
	void setNextState(std::string state);
	bool IsType(const std::string &type);

	void stepGraphics(std::string direction);
	void stepGraphicsMouse();
	bool clickHandler();

	void InitResolutions();

private:
	std::string next_state;
	Entity* logotype;
	std::map<std::string, std::vector<std::string>> resolutions;
	std::vector<Text*> menu_texts;
	int menu_texts_padding;
	std::vector<Text*>::iterator current_selected;
	std::vector<Text*>::iterator last_selected;
};