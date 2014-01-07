#include <stdio.h>
#include <map>
#include <fstream>
#include <sstream>

#include "GFXState.h"
#include "Entity.h"
#include "DrawManager.h"
#include "Text.h"
#include "Config.h"
#include "InputManager.h"
#include "Sound.h"
#include "Utils.h"

GFXState::GFXState() {
	logotype = nullptr;
	next_state = "";
	menu_texts_padding = Config::getInt("menu_text_padding", 100);
};

bool GFXState::Init() {
	printf("GFX State entered");
	logotype = new Entity("logo.png", 0, 0, 460, 67);
	logotype->Create(spriteManager, 0, 0);
	logotype->setX(drawManager->getWidth() * 0.5f - logotype->getWidth() * 0.5f);
	logotype->setY(100);
	InitResolutions();
	/*current_selected = menu_texts.begin();
	last_selected = menu_texts.begin();
	for(auto it = menu_texts.begin(); it != menu_texts.end(); ++it) {
		if(current_selected == it) {
			(*it)->setColor(Config::getHex("menu_current_color", 0xFF0000));
		} else {
			(*it)->setColor(Config::getHex("menu_idle_color", 0x000000));
		}
		(*it)->setX((*it)->getX() - (*it)->getWidth() * 0.5f);
	}*/
	return true;
}

void GFXState::Exit() {

}

bool GFXState::Update() {
	/*if(InputManager::pressedWithoutRepeat(KEY_DOWN)) {
		stepGraphics("down");
	} else if(InputManager::pressedWithoutRepeat(KEY_UP)) {
		stepGraphics("up");
	}
	
	stepGraphicsMouse();
	return clickHandler();
	*/
	return true;
}

void GFXState::Draw() {
	logotype->draw(drawManager);

	for(auto txt : menu_texts) {
		txt->draw();
	}
}

void GFXState::stepGraphics(std::string direction) {
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

void GFXState::stepGraphicsMouse() {
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
			last_selected = it;
			break;
		}
	}
}

bool GFXState::clickHandler() {
	enum MenuValues {
		
	};
	if(InputManager::pressedWithoutRepeat(KEY_SPACE) || InputManager::pressedWithoutRepeat(KEY_KEYPAD_ENTER)) {
		if(!(*current_selected)) return true;

		
			

			//themes goes here

			//Minimalistic --- New Age?
			//Deep Space --- Vangelis Alpha
			//Sci-Fi --- Techno or something
			//Fantsy --- Violin or something
		
		return false;
	}
	return true;
}

void GFXState::InitResolutions() {
	menu_texts.clear();
	std::ifstream stream(Config::get("resolution_path"));
	if(!stream.is_open()) printf("Can't open %s\n", Config::get("resolution_path"));

	std::string row;
	std::vector<std::string> str;
	std::string aspect_ratio = "";
	str.clear();
	
	while(!stream.eof()) {
		std::getline(stream, row, '\n');
		if(row.empty()) continue;
		std::stringstream stringstream(row);
		Utils::split(row, ':', str);
		if(str.size() == 2) {
			// An aspect ratio
			aspect_ratio = str.front() + ":" + str.back();
			std::vector<std::string> new_vec;
			resolutions.insert(std::pair<std::string, std::vector<std::string>>(aspect_ratio, new_vec));
		} else {
			// A resultion
			auto it = resolutions.find(aspect_ratio);
			if(it != resolutions.end()) {
				it->second.push_back(row);
			}
		}
		str.clear();
	}

	int i = 1;
	for(auto aspect_ratio : resolutions) {
		Text* ar_text = new Text(100, 300 + menu_texts_padding * i, "Arial", 28, aspect_ratio.first, drawManager->getRenderer());
		menu_texts.push_back(ar_text);
		int j = 1;
		for(auto res : aspect_ratio.second) {
			Text* txt = new Text(100 + j * 120, 300 + menu_texts_padding * i, "Arial", 20, res, drawManager->getRenderer());
			menu_texts.push_back(txt);
			j++;
		}
		i++;
	}

	printf("All resolutions loaded!\n");
}

std::string GFXState::Next() {
	return next_state;
};

void GFXState::setNextState(std::string state) {
	next_state = state;
}

bool GFXState::IsType(const std::string &type) {
	return type.compare("GFXState") == 0;
}