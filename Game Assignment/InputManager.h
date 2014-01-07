#pragma once

#include "SDL.h"
#include "InputDefinitions.h"

class InputManager {
public:
    InputManager();
    static void update();
	static bool pressedWithoutRepeat(KeyboardKey key);
    static bool pressed(KeyboardKey key);
	static bool mousePressed(MouseButton mouse);
	static bool mousePressedOnce(MouseButton mouse);

    static const uint8_t* keyboard;
	static bool previously_pressed_keys[KEYBOARD_SIZE];
    static uint32_t mouse;
	static bool previously_pressed_btns[MOUSE_MAX];
    static int mouseX;
    static int mouseY;
    static bool keyDown[KEYBOARD_SIZE];
    static bool keyUp[KEYBOARD_SIZE];
    static bool mouseDown[MOUSE_MAX];
    static bool mouseUp[MOUSE_MAX];
};
