#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "SDL.h"
#include "Input.h"
#include "Utils.h"
#include "Game.h"
#include "DrawManager.h"
#include "CollisionManager.h"
#include "Player.h"
#include "Brick.h"
#include "Ball.h"
#include "Particle.h"

struct Rect {
	float top, bottom, left, right;
};

Game::Game() {
	level = 1;
	player = nullptr;
	balls.clear();
	particles.clear();
};

bool Game::Init() {
	InitBricks();
	player = new Player("paddle_normal.png", (drawManager->getWidth() * 0.5) - 75, drawManager->getHeight() - 50, 150, 30);
	player->Create(spriteManager, 0, 0);

	Ball* ball = new Ball("ball.png", (drawManager->getWidth() * 0.5) - 16, drawManager->getHeight() - 50 - 32, 32, 32);
	ball->Create(spriteManager, 0, 0);
	ball->setSpeed(1);
	ball->setDirection(Utils::Random::frandom(-0.5, 0.5), -1.0f);
	ball->freeze();
	balls.push_back(ball);

	/* Set the mouse to the center of the screen */
	SDL_WarpMouseInWindow(window, drawManager->getWidth()/2, drawManager->getHeight()/2);
	
	return true;
}

void Game::Exit() {

	if(balls.size() > 0) {
		auto it = balls.begin();
		while(it != balls.end()) {
			delete (*it);
			++it;
		};
	}
	balls.clear();

	if(particles.size() > 0) {
		auto it = particles.begin();
		while(it != particles.end()) {
			delete (*it);
			++it;
		};
	}
	particles.clear();
	
	delete player;
	player = nullptr;

	printf("Game Exit\n");
}

bool Game::Update() {
	/**
	*	RELEASE BALL
	*/
	if(keyboard->IsDownOnce(SDLK_SPACE)) {
		for(auto it = balls.begin(); it != balls.end(); ++it) {
			(*it)->unfreeze();
		}
	}

	/**
	*	PLAYER MOVEMENT
	*/
	printf("%f", deltatime);
	player->setX(mouse->GetX()-(player->getWidth()/2));

	/**
	*	PLAYER COLLISION WITH WINDOW BOUNDS
	*/
	if(player->getX() < 0) { player->setX(0.0f); }
	if(player->getY() < 0) { player->setY(0.0f); }
	if(player->getX() + player->getWidth() > this->drawManager->getWidth()) { player->setX(this->drawManager->getWidth() - player->getWidth()); }
	if(player->getY() + player->getHeight() > this->drawManager->getHeight()) { player->setY(this->drawManager->getHeight() - player->getHeight()); }
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		(*it)->move(1.0);
	}

	/**
	*	BALL(S) COLLISION WITH BOUNDS
	*/
	std::vector<Ball*> colliding_balls;
	colliding_balls.clear();

	for(auto it = balls.begin(); it != balls.end(); ++it) {
		if((*it)->getX() < 0) {
			(*it)->setX(0.0f);
			(*it)->setDirectionX((*it)->getDirectionX()*-1);
			colliding_balls.push_back((*it));
		}

		if((*it)->getX() + (*it)->getWidth() > drawManager->getWidth()) {
			(*it)->setX((float)(drawManager->getWidth() - (*it)->getWidth()));
			(*it)->setDirectionX((*it)->getDirectionX()*-1);
			colliding_balls.push_back((*it));
		}
		
		if((*it)->getY() < 0) {
			(*it)->setY(0.0f);			
			(*it)->setDirectionY((*it)->getDirectionY()*-1);
			colliding_balls.push_back((*it));
		}
		
		if((*it)->getY() + (*it)->getHeight() > drawManager->getHeight()) {
			(*it)->setY((float)(drawManager->getHeight() - (*it)->getHeight()));			
			(*it)->setDirectionY((*it)->getDirectionY()*-1);
			colliding_balls.push_back((*it));
		}
	}
	
	if(colliding_balls.size() > 0) {
		for(auto it = colliding_balls.begin(); it != colliding_balls.end(); ++it) {
			SpawnParticles(100, (*it)->getX(), (*it)->getY());
		}		
	}

	/**
	*	BALL COLLISION WITH PADDLE
	*/
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		(*it)->move(1.0);
	}

	/**
	*	UPDATE PARTICLES
	*/
	auto it = particles.begin();
	while(it != particles.end()) {
		if((*it)->isDead()) {
			it = particles.erase(it);
		} else {
			(*it)->update();
			if((*it)->getX() < 0 || (*it)->getX() > drawManager->getWidth()) (*it)->setVelX((*it)->getVelX()*-1);
			if((*it)->getY() < 0 || (*it)->getY() > drawManager->getHeight()) (*it)->setVelY((*it)->getVelY()*-1);
			++it;
		}
	}

	/**
	*	RESTART GAME
	*/
	if(this->keyboard->IsDownOnce(SDLK_r)) {
		next_state = "Game";
		return false;
	}

	return true;
}

void Game::Draw() {
	/**
	*	DRAW PLAYER
	*/
	player->draw(drawManager);
	
	for(auto y = bricks.begin(); y != bricks.end(); ++y) {
		for(auto x = (*y).begin(); x != (*y).end(); ++x) {
			(*x)->draw(drawManager);
		}
	}

	for(auto it = balls.begin(); it != balls.end(); ++it) {
		(*it)->draw(drawManager);
	}

	for(auto it = balls.begin(); it != balls.end(); ++it) {
		(*it)->draw(drawManager);
	}

	for(auto it = particles.begin(); it != particles.end(); ++it) {
		(*it)->draw(drawManager);
	}
}

void Game::SpawnParticles(unsigned int amount, int x, int y) {
	for(int i = 0; i < amount; i++) {
		Particle* part = new Particle(x,y, Utils::Random::frandom(-1.0, 1.0), Utils::Random::frandom(-1.0, 1.0), Utils::Random::frandom(0.5, 0.6), Utils::Random::random(100, 700), 0);
		particles.push_back(part);
	}
}

bool Game::InitBricks() {
	// LOAD BRICK INFO
	std::map<char, std::string> brick_info;
	std::string row;
	std::ifstream fstream("../maps/bricks.txt");
	if(!fstream.is_open()) return false;
	while(!fstream.eof()) {
		std::getline(fstream, row);
		std::stringstream stringstream(row);
		char id;
		stringstream >> id;
		std::string filename;
		stringstream >> filename;
		brick_info.insert(std::pair<char, std::string>(id, filename));
	}

	std::string path = "../maps/" + Utils::int_to_string(level) + ".txt";
	fstream.close();

	fstream.open(path.c_str());

	if(!fstream.is_open()) return false;

	int brick_width;
	int brick_height;
	int row_size;

	fstream >> row_size >> brick_width >> brick_height;

	int x = 0;
	int y = 0;
	row = "";
	while(!fstream.eof()) {
		std::getline(fstream, row, '\n');
		if(row.length() == 0) continue;

		std::vector<Brick*> r;

		x = 0;
		for(int i = 0; i < row.length(); i++) {
			std::map<char, std::string>::iterator it = brick_info.find(row[i]);
			if(it == brick_info.end()) continue;
			Brick *brick = new Brick(it->second, x, y, brick_width, brick_height);
			brick->Create(spriteManager, 0, 0);
			r.push_back(brick);
			x += brick_width;
		}
		bricks.push_back(r);
		y += brick_height;
	}

	return true;
}

std::string Game::Next() {
	return next_state;
};

void Game::setNextState(std::string state) {
	next_state = state;
}

bool Game::IsType(const std::string &type) {
	return type.compare("Game") == 0;
};