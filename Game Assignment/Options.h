#pragma once

#include <vector>
#include "State.h"

class Entity;
class Text;

class Options: public State {
public:
	Options();
	bool Init();
	void Exit();

	bool Update();
	void Draw();

	std::string Next();
	void setNextState(std::string state);
	bool IsType(const std::string &type);

	void stepOptions(std::string direction);
	void stepOptionsMouse();
	bool clickHandler();

private:
	std::string next_state;
	Entity* logotype;
	std::vector<Text*> menu_texts;
	int menu_texts_padding;
	std::vector<Text*>::iterator current_selected;
	std::vector<Text*>::iterator last_selected;
};