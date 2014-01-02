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
	player = new Player("paddle_normal.png", (drawManager->getWidth() * 0.5) - 50, drawManager->getHeight() - 50, 100, 20);
	player->Create(spriteManager, 0, 0);

	Ball* ball = new Ball("ball.png", (drawManager->getWidth() * 0.5) - 10, drawManager->getHeight() - 50 - 20, 20, 20);
	ball->Create(spriteManager, 0, 0);
	ball->setSpeed(16.0);
	ball->setDirection(Utils::Random::frandom(-0.8, 0.8), -1.0f);
	ball->freeze();
	balls.push_back(ball);

	/* Set the mouse to the center of the screen */
	SDL_WarpMouseInWindow(window, drawManager->getWidth()/2, drawManager->getHeight()/2);
	
	return true;
}

void Game::Exit() {

	/* Delallocate all of our balls */
	if(balls.size() > 0) {
		auto it = balls.begin();
		while(it != balls.end()) {
			delete (*it);
			++it;
		};
	}
	balls.clear();

	/* Deallocate all of the particles(might be removed in future because current particles sucks) */
	if(particles.size() > 0) {
		auto it = particles.begin();
		while(it != particles.end()) {
			delete (*it);
			++it;
		};
	}
	particles.clear();

	/* Deallocate bricks */
	/*if(bricks.size() > 0) {
		auto it = particles.begin();
		while(it != particles.end()) {
			delete (*it);
			++it;
		};
	}*/
	particles.clear();
	
	/* Deallocate player */
	delete player;
	player = nullptr;

	printf("Game Exit\n");
}

bool Game::Update() {
	/**
	*	RELEASE BALL
	*/
	if(mouse->IsDownOnce(MB_LEFT)) {
		for(auto it = balls.begin(); it != balls.end(); ++it) {
			(*it)->unfreeze();
		}
	}

	/**
	*	PLAYER MOVEMENT
	*/
	player->setX(mouse->GetX()-(player->getWidth()/2));
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		if((*it)->isFreezed()) {
			(*it)->setX(mouse->GetX() - (*it)->getWidth()/2);
		}
	}

	/**
	*	PLAYER COLLISION WITH WINDOW BOUNDS
	*/
	if(player->getX() < 0) { player->setX(0.0f); }
	if(player->getY() < 0) { player->setY(0.0f); }
	if(player->getX() + player->getWidth() > this->drawManager->getWidth()) { player->setX(this->drawManager->getWidth() - player->getWidth()); }
	if(player->getY() + player->getHeight() > this->drawManager->getHeight()) { player->setY(this->drawManager->getHeight() - player->getHeight()); }

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
	*	BALL COLLISION WITH BRICKS
	*/
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		for(auto b = bricks.begin(); b != bricks.end(); ++b) {

			/* Return overlap/overflow with collision */
			std::vector<float> overflow(2, 0);
			
			if(CollisionManager::collideRectPlus((*it)->getBounds(), (*b)->getBounds(), overflow)) {

				/* If there are any overlap in any axis (if its colliding) */
				if(overflow[0] != 0 || overflow[1] != 0) {
					/* Move */
					
					(*it)->move(overflow[0], overflow[1]);

					/* Change direction of ball */
					if(overflow[0] > overflow[1])
						(*it)->setDirectionX((*it)->getDirectionX()*-1);
					else
						(*it)->setDirectionY((*it)->getDirectionY()*-1);
				
					// Remove block
					delete (*b);
					b = bricks.erase(b);
					--b;
				}
			}
		}
	}

	/* Move balls */
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		(*it)->update((*it)->getSpeed() * deltatime);
		std::vector<float> overflow(2, 0);
		
		// Ball collision with paddle
		// Here we calculate the new x-velocity based on where on the paddle the ball bounce
		if(CollisionManager::collideRectPlus((*it)->getBounds(), player->getBounds(), overflow)) {
			if(overflow[0] != 0 || overflow[1] != 0) {
				/* Move */
				printf("Overflow-Y: %f, ", overflow[1]);
				(*it)->move(overflow[0], overflow[1]);

				/* Change direction of ball */
				if(overflow[1] < 0) {
					(*it)->setDirectionY((*it)->getDirectionY()*-1);
					
					// Here happens the magic
					// Basicly it says
					//
					// x_velocity = (ball_center - paddle_center) / (ball_width / 2)
					//
					(*it)->setDirectionX((((*it)->getX() + (*it)->getWidth()*.5) - (player->getX() + player->getWidth()*.5))/player->getWidth()*.5);
				}
			}
		}
	}

	/**
	*	UPDATE PARTICLES
	*/
	auto it = particles.begin();
	while(it != particles.end()) {
		if((*it)->isDead()) {
			it = particles.erase(it);
		} else {
			(*it)->update(deltatime);
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
	
	for(auto it = bricks.begin(); it != bricks.end(); ++it) {
		(*it)->draw(drawManager);
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
		Particle* part = new Particle(x,y, Utils::Random::frandom(-1.0, 1.0), Utils::Random::frandom(-1.0, 1.0), Utils::Random::frandom(100.0, 200.0), Utils::Random::random(100, 700), 0);
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
	columns = row_size;

	int x = 0;
	int y = 0;
	row = "";
	int count_rows = 0;
	while(!fstream.eof()) {
		std::getline(fstream, row, '\n');
		if(row.length() == 0) continue;

		x = 0;
		for(int i = 0; i < row.length(); i++) {
			std::map<char, std::string>::iterator it = brick_info.find(row[i]);
			if(it == brick_info.end()) continue;
			Brick *brick = new Brick(it->second, x, y, brick_width, brick_height);
			brick->Create(spriteManager, 0, 0);
			bricks.push_back(brick);
			x += brick_width;
		}
		y += brick_height;
		count_rows++;
	}

	rows = count_rows;

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