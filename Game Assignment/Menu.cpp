#include "Menu.h"

Menu::Menu() {
	
};

bool Menu::Init() {
	printf("Menu Init\n");
	return true;
}

void Menu::Exit() {
	printf("Menu Exit\n");
}


bool Menu::Update() {
	printf("Menu update\n");
	// If we wanna change to any state from here
	/**
	*	If we wanna change from any state to any state,
	*	set next stage e.g. next_state = "Game"; and return false in Update method
	*/

	next_state = "Game";
	return false;

	return true;
}

void Menu::Draw() {

}

std::string Menu::Next() {
	return next_state;
};

void Menu::setNextState(std::string state) {
	next_state = state;
}

bool Menu::IsType(const std::string &type) {
	return type.compare("Menu") == 0;
};