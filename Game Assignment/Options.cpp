#include "Options.h"
#include "Entity.h"
#include "DrawManager.h"
#include "Text.h"
#include "Config.h"
#include "InputManager.h"
#include "Sound.h"

#include <iostream>

Options::Options() {
	logotype = nullptr;
	next_state = "";
	menu_texts_padding = Config::getInt("menu_text_padding", 100);
};

bool Options::Init() {
	logotype = new Entity("logo.png", 0, 0, 460, 67);
	logotype->Create(spriteManager, 0, 0);
	logotype->setX(drawManager->getWidth() * 0.5f - logotype->getWidth() * 0.5f);
	logotype->setY(100);

	menu_texts.push_back(new Text((int)(drawManager->getWidth() * 0.5f), logotype->getBottom() + menu_texts_padding, "franklin_gothic_demi", 32, "GRAPHICS", drawManager->getRenderer()));
	menu_texts.push_back(new Text((int)(drawManager->getWidth() * 0.5f), (int)(menu_texts.back()->getY() + menu_texts_padding), "franklin_gothic_demi", 32, "CONTROLLS", drawManager->getRenderer()));
	menu_texts.push_back(new Text((int)(drawManager->getWidth() * 0.5f), (int)(menu_texts.back()->getY() + menu_texts_padding), "franklin_gothic_demi", 32, "SOUND", drawManager->getRenderer()));
	menu_texts.push_back(new Text((int)(drawManager->getWidth() * 0.5f), (int)(menu_texts.back()->getY() + menu_texts_padding), "franklin_gothic_demi", 32, "BACK", drawManager->getRenderer()));

	current_selected = menu_texts.begin();
	last_selected = menu_texts.begin();
	for(auto it = menu_texts.begin(); it != menu_texts.end(); ++it) {
		if(current_selected == it) {
			(*it)->setColor(Config::getHex("menu_current_color", 0xFF0000));
		} else {
			(*it)->setColor(Config::getHex("menu_idle_color", 0x000000));
		}
		(*it)->setX((*it)->getX() - (*it)->getWidth() * 0.5f);
	}
	printf("Entered options\n");
	return true;
}

void Options::Exit() {	
	if(menu_texts.size() > 0) {
		auto it = menu_texts.begin();
		while(it != menu_texts.end()) {
			delete (*it);
			++it;
		};
	}
	menu_texts.clear();

	delete logotype;
	logotype = nullptr;
}

bool Options::Update() {
	if(InputManager::pressedWithoutRepeat(KEY_DOWN)) {
		stepOptions("down");
	} else if(InputManager::pressedWithoutRepeat(KEY_UP)) {
		stepOptions("up");
	}
	
	stepOptionsMouse();
	return clickHandler();
}

void Options::Draw() {
	logotype->draw(drawManager);

	for(auto text : menu_texts) {
		text->draw();
	}
}

void Options::stepOptions(std::string direction) {
	if(!Config::getBool("menu_keyboard_select", true)) return;
	SoundManager::get("menu_select")->stop();
	SoundManager::get("menu_select")->play();
	std::vector<Text*>::iterator prev_current = current_selected;
	
	if(direction == "down") {
		if(current_selected == menu_texts.end()-1) {
			current_selected = menu_texts.begin();
		} else {
			++current_selected;
		}
	} else if(direction == "up") {		
		if(current_selected == menu_texts.begin()) {
			current_selected = menu_texts.end();
		}
		--current_selected;
	}

	(*current_selected)->setColor(Config::getHex("menu_current_color", 0xFF0000));
	(*prev_current)->setColor(Config::getHex("menu_idle_color", 0xFF0000));
}

void Options::stepOptionsMouse() {
	if(!Config::getBool("menu_mouse_select", true)) return;
	for(auto it = menu_texts.begin(); it != menu_texts.end(); ++it) {
		if((*it)->inside((float)InputManager::mouseX, (float)InputManager::mouseY)) {
			if(it != last_selected) {
				SoundManager::get("menu_select")->play();
			}
			(*it)->setColor(Config::getHex("menu_current_color", 0xFF0000));
			
			for(auto it2 = menu_texts.begin(); it2 != menu_texts.end(); ++it2) {
				if(it2 == it) continue;
				(*it2)->setColor(Config::getHex("menu_idle_color", 0xFF0000));
			}
			current_selected = it;
			last_selected = it;
			break;
		}
	}
}

bool Options::clickHandler() {
	enum MenuValues {
		GFX,
		CONTROLS,
		AUDIO,
		BACK,
	};
	if(InputManager::pressedWithoutRepeat(KEY_SPACE) || InputManager::pressedWithoutRepeat(KEY_ENTER)) {
		if(!(*current_selected)) return true;
		
		switch((current_selected - menu_texts.begin())) {
			case GFX:
				setNextState("GFXState");
			break;
			case CONTROLS:
				setNextState("OptionControls");
				break;
			case AUDIO:
				setNextState("OptionAudio");
				break;
			case BACK:
				setNextState("Menu");
				break;
		}
		return false;
	}

	if(InputManager::mousePressedOnce(MOUSE_LEFT)) {
		for(auto it = menu_texts.begin(); it != menu_texts.end(); ++it) {
			if((*it)->inside(InputManager::mouseX, InputManager::mouseY)) {
				switch((it - menu_texts.begin())) {
					case GFX:
						setNextState("GFXState");
					break;
					case CONTROLS:
						setNextState("OptionControls");
						break;
					case AUDIO:
						setNextState("OptionAudio");
						break;
					case BACK:
						setNextState("Menu");
						break;
				}
				return false;
			}
		}
	}
	return true;
}

std::string Options::Next() {
	return next_state;
};

void Options::setNextState(std::string state) {
	next_state = state;
}

bool Options::IsType(const std::string &type) {
	return type.compare("Options") == 0;
}