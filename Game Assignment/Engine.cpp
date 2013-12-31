#include "Engine.h"
#include "State.h"
#include "Input.h"
#include "DrawManager.h"
#include "SpriteManager.h"
#include "Config.h"
#include "Utils.h"
#include "SDL.h"

Engine::Engine() {
	current = nullptr;
	window = nullptr;
	drawManager = nullptr;
	spriteManager = nullptr;
	keyboard = nullptr;
	mouse = nullptr;
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
	width = Config::defined("width") ? Utils::string_to_int(Config::get("width")) : 800;
	height = Config::defined("height") ? Utils::string_to_int(Config::get("height")) : 600;

	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	if(window == nullptr) { printf("Engine::Init() Can't create window\n"); return false; };
	
	drawManager = new DrawManager(window, width, height);
	if(drawManager == nullptr) { printf("Engine::Init() Can't create draw manager\n"); return false; }

	spriteManager = new SpriteManager(drawManager);
	if(!spriteManager->Init("../assets/gfx/")) { printf("Engine::Init() Can't initialize sprite manager\n"); return false; }

	keyboard = new Keyboard;
	mouse = new Mouse;

	printf("Engine Init\n");
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

	delete keyboard;
	keyboard = nullptr;

	delete mouse;
	mouse = nullptr;

	current->Exit();
	current = nullptr;

	printf("Engine Cleanup\n");
	this->Quit();
}

void Engine::UpdateTime() {
	current->ticks++;
	current->deltatime = 0.1f;
}

void Engine::HandleEvents() 
{
	SDL_Event event;
	while(SDL_PollEvent(&event)) {
		if(event.type == SDL_QUIT) {
			this->Cleanup();
		} else if(event.type == SDL_KEYDOWN) {
			int index = event.key.keysym.sym & 0xFF;
			keyboard->m_current[index] = true;
		} else if(event.type == SDL_KEYUP) {
			int index = event.key.keysym.sym & 0xFF;
			keyboard->m_current[index] = false;
		} else if(event.type == SDL_MOUSEMOTION) {
			mouse->m_x = event.motion.x;
			mouse->m_y = event.motion.y;
		} else if(event.type == SDL_MOUSEBUTTONDOWN) {
			if(event.button.button == SDL_BUTTON_LEFT) {
				mouse->m_current[0] = true;
			} else if(event.button.button == SDL_BUTTON_RIGHT) {
				mouse->m_current[1] = true;
			};
		}
		else if(event.type == SDL_MOUSEBUTTONUP) {
			if(event.button.button == SDL_BUTTON_LEFT) {
				mouse->m_current[0] = false;
			} else if(event.button.button == SDL_BUTTON_RIGHT) {
				mouse->m_current[1] = false;
			};
		};
	}
}

void Engine::Update() 
{
	if(current == nullptr) { return; };
	UpdateTime();
	if(!current->Update()) {
		ChangeState();
	};
	mouse->PostUpdate();
	keyboard->PostUpdate();
}

void Engine::Draw() 
{
	if(current == nullptr) { return; };
	drawManager->Clear();
	current->Draw();
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
			current->mouse = mouse;
			current->keyboard = keyboard;
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

	for(unsigned int i = 0; i < states.size(); i++) {
		if(states[i]->IsType(next)) {
			current = states[i];
			current->mouse = mouse;
			current->keyboard = keyboard;
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