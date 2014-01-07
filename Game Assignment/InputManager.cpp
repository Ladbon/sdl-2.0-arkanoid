#include "InputManager.h"

const uint8_t* InputManager::keyboard;
uint32_t InputManager::mouse;
bool InputManager::previously_pressed_keys[KEYBOARD_SIZE];
bool InputManager::previously_pressed_btns[MOUSE_MAX];
int InputManager::mouseX;
int InputManager::mouseY;
bool InputManager::keyDown[KEYBOARD_SIZE];
bool InputManager::keyUp[KEYBOARD_SIZE];
bool InputManager::mouseDown[MOUSE_MAX];
bool InputManager::mouseUp[MOUSE_MAX];

InputManager::InputManager() {
	keyboard = nullptr;
	mouse = 0;
	mouseX = 0;
	mouseY = 0;
	
	for(int i = 0; i < KEYBOARD_SIZE; i++) {
		previously_pressed_keys[i] = false;
	}
}

void InputManager::update() {
	int i;
	for(i = 0; i < KEYBOARD_SIZE; i++) {
		keyDown[i] = false;
		keyUp[i] = false;
	}

	for(i = 0; i < MOUSE_MAX; i++) {
		mouseDown[i] = false;
		mouseUp[i] = false;
	}
}

bool InputManager::pressedWithoutRepeat(KeyboardKey key) {
	if(pressed(key)) {
		if(!previously_pressed_keys[key]) {			
			previously_pressed_keys[key] = true;
			return true;
		}
		return false;
	} else {
		previously_pressed_keys[key] = false;
		return false;
	}
}

bool InputManager::pressed(KeyboardKey key) {
	if(!keyboard) return false;
	int sdl_key = static_cast<int>(key);
	if(keyboard[sdl_key])
		return true;
	return false;
}

bool InputManager::mousePressed(MouseButton _mouse) {
	switch(_mouse) {
	case MOUSE_LEFT:
		if(mouse & SDL_BUTTON(1))
			return true;
		break;
	case MOUSE_RIGHT:
		if(mouse & SDL_BUTTON(3))
			return true;
		break;
	default:
		break;
	}
	return false;
}

bool InputManager::mousePressedOnce(MouseButton mouse) {
	if(mousePressed(mouse)) {
		if(!previously_pressed_btns[mouse]) {			
			previously_pressed_btns[mouse] = true;
			return true;
		}
		return false;
	} else {
		previously_pressed_btns[mouse] = false;
		return false;
	}
}