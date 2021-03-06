#include "Engine.h"
#include "State.h"
#include "InputManager.h"
#include "DrawManager.h"
#include "SpriteManager.h"
#include "Config.h"
#include "Utils.h"
#include "SDL.h"
#include "Entity.h"
#include "Text.h"

Engine::Engine() {
	current = nullptr;
	window = nullptr;
	drawManager = nullptr;
	spriteManager = nullptr;
	m_running = true;
	cursor = nullptr;
	debug = nullptr;
};

Engine::~Engine() {
	auto it = states.begin();
	while(it != states.end()) {
		delete (*it);
		++it;
	};
};

bool Engine::Init(const char* title, int width, int height) {
	/* If width and height are defined in config file, use those */
	width = Config::getInt("resolution_x", 1920);
	height = Config::getInt("resolution_y", 1080);

	SDL_Init(SDL_INIT_EVERYTHING);
	int flags = SDL_WINDOW_OPENGL;
	if(Config::getBool("fullscreen", false)) {
		flags += SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, flags);
	if(window == nullptr) { printf("Engine::Init() Can't create window\n"); return false; };
	
	drawManager = new DrawManager(window, width, height);
	if(drawManager == nullptr) { printf("Engine::Init() Can't create draw manager\n"); return false; }

	spriteManager = new SpriteManager(drawManager);
	if(!spriteManager->Init(Config::get("gfx_path"))) { printf("Engine::Init() Can't initialize sprite manager\n"); return false; }
	
	// Create our wonderful cursor
	cursor = new Entity("cursor.png", InputManager::mouseX, InputManager::mouseY, 28, 29);
	cursor->Create(spriteManager, 0, 0);

	SDL_ShowCursor(0);

	debug = new Text(0, 0, "Arial", 12, "", drawManager->getRenderer());
	return true;
}

void Engine::Cleanup()
{
	if(spriteManager != nullptr) {
		spriteManager->Cleanup();
		delete spriteManager;
		spriteManager = nullptr;
	}

	if(drawManager != nullptr) {
		drawManager->Cleanup();
		delete drawManager;
		drawManager = nullptr;
	}

	if(window != nullptr) {
		SDL_DestroyWindow(window);
		window = nullptr;
	}

	if(current != nullptr) {
		current->Exit();
		current = nullptr;
	}

	if(cursor != nullptr) {
		delete cursor;
		cursor = nullptr;
	}

	delete debug;
	debug = nullptr;
	this->Quit();
}

void Engine::UpdateTime() {
	unsigned int ticks = SDL_GetTicks();
	unsigned int delta = ticks - current->ticks;
    current->ticks = ticks;
	current->deltatime = (float)delta * 0.001f;
}

void Engine::HandleEvents() {
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			this->Cleanup();
		} else if(event.type == SDL_KEYDOWN) {
			
			InputManager::keyboard = SDL_GetKeyboardState(nullptr);
			int index = event.key.keysym.scancode;
			InputManager::keyDown[index] = true;

		} else if(event.type == SDL_KEYUP) {
			InputManager::keyboard = SDL_GetKeyboardState(nullptr);
			int index = event.key.keysym.scancode;
			InputManager::keyUp[index] = true;
		} else if(event.type == SDL_MOUSEMOTION) {
			
			InputManager::mouseX = event.motion.x;
			InputManager::mouseY = event.motion.y;

		} else if(event.type == SDL_MOUSEBUTTONDOWN) {
			InputManager::mouse = SDL_GetMouseState(nullptr, nullptr);
		}
		else if(event.type == SDL_MOUSEBUTTONUP) {
			InputManager::mouse = SDL_GetMouseState(nullptr, nullptr);
		};

		if(event.type == SDL_WINDOWEVENT) {
			switch(event.window.event) {
			case SDL_WINDOWEVENT_FOCUS_LOST:
				
				break;
			case SDL_WINDOWEVENT_FOCUS_GAINED:
				
				break;
			}
		}
	}
	cursor->setX(InputManager::mouseX);
	cursor->setY(InputManager::mouseY);

	debug->setText("mouse_x: " + Utils::int_to_string(InputManager::mouseX) + "\n" + 
		"mouse_y" + Utils::int_to_string(InputManager::mouseY));
}

void Engine::Update() {
	if(current == nullptr) { return; };
	UpdateTime();
	if(!current->Update()) {
		ChangeState();
	};
	InputManager::update();
}

void Engine::Draw() 
{
	if(current == nullptr) { return; };
	drawManager->Clear();
	current->Draw();
	cursor->draw(drawManager);
	debug->draw();
	drawManager->Present();
	SDL_Delay(2);
}

// STATE RELATED METHODS
void Engine::Attach(State *state) {
	states.push_back(state);
}

void Engine::SetState(const std::string &type) {
	for(unsigned int i = 0; i < states.size(); i++) {
		if(states[i]->IsType(type)) {
			current = states[i];
			current->drawManager = this->getDrawManager();
			current->spriteManager = this->getSpriteManager();
			current->deltatime = 0.0f;
			current->ticks = 0;
			current->window = window;
			current->Init();
		};
	};
}

void Engine::ChangeState() {
	std::string next = current->Next();
	if(current != nullptr) {
		current->Exit();
		current = nullptr;
	};
	if(next.empty()) { this->Cleanup(); return; }

	printf("Changing state to: %s\n", next.c_str());

	for(unsigned int i = 0; i < states.size(); i++) {
		if(states[i]->IsType(next)) {
			current = states[i];
			current->drawManager = this->getDrawManager();
			current->spriteManager = this->getSpriteManager();
			current->deltatime = 0.0f;
			current->ticks = 0;
			current->window = window;
			current->Init();
			return;
		};
	};
}