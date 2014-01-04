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
#include "Config.h"
#include "Text.h"
#include "Color.h"

struct Rect {
	float top, bottom, left, right;
};

Game::Game() {
	level = 1;
	player = nullptr;
	balls.clear();
	life_text = nullptr;
	score_text = nullptr;
};

bool Game::Init() {
	printf("Game init\n");

	// Set the mouse to the center of the screen
	SDL_WarpMouseInWindow(window, drawManager->getWidth()/2, drawManager->getHeight()/2);

	// Creates the destroyable bricks
	InitBricks();
	
	// Create player(paddle)
	player = new Player("paddle_normal.png", (float)(drawManager->getWidth() * 0.5) - 50, (float)(drawManager->getHeight() - 50), 100, 20);
	player->Create(spriteManager, 0, 0);

	// Spawn start(ing) ball(s)
	int start_balls = 1;
	if(Config::defined("start_balls_amount")) start_balls = Utils::string_to_int(Config::get("start_balls_amount"));	
	for(int i = 0; i < start_balls; i++) {
		spawnBall();
	}
	
	// Init text objects
	life_text = new Text(10, drawManager->getHeight() - 50, "Arial", 20, "Lifes: " + Utils::int_to_string(player->getLifes()), drawManager->getRenderer());
	score_text = new Text(drawManager->getWidth() - 100, drawManager->getHeight() - 50, "Arial", 20, "Score: 0", drawManager->getRenderer());
	return true;
}

void Game::Exit() {
	// Remove balls
	if(balls.size() > 0) {
		auto it = balls.begin();
		while(it != balls.end()) {
			delete (*it);
			++it;
		};
	}
	balls.clear();

	// Remove bricks
	if(bricks.size() > 0) {
		auto it = bricks.begin();
		while(it != bricks.end()) {
			delete (*it);
			++it;
		};
	}

	// Texts
	delete life_text;
	life_text = nullptr;

	delete score_text;
	score_text = nullptr;

	// Remove player
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

	// Player movement
	player->setX((float)(mouse->GetX()-(player->getWidth()*0.5f)));
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		if((*it)->isFreezed()) {
			(*it)->setX((float)(mouse->GetX() - (*it)->getWidth()*0.5f));
		}
	}

	// Player collision with bounds
	if(player->getX() < 0) { player->setX(0.0f); }
	if(player->getY() < 0) { player->setY(0.0f); }
	if(player->getX() + player->getWidth() > this->drawManager->getWidth()) { 
			player->setX((float)(this->drawManager->getWidth() - player->getWidth())); 
	}
	if(player->getY() + player->getHeight() > this->drawManager->getHeight()) { 
		player->setY((float)(this->drawManager->getHeight() - player->getHeight())); 
	}

	/**
	*	BALL(S) COLLISION WITH BOUNDS
	*/

	for(auto it = balls.begin(); it != balls.end(); ++it) {
		if((*it)->getX() < 0) {
			(*it)->setX(0.0f);
			(*it)->setDirectionX((*it)->getDirectionX()*-1);
		}

		if((*it)->getX() + (*it)->getWidth() > drawManager->getWidth()) {
			(*it)->setX((float)(drawManager->getWidth() - (*it)->getWidth()));
			(*it)->setDirectionX((*it)->getDirectionX()*-1);
		}
		
		if((*it)->getY() < 0) {
			(*it)->setY(0.0f);			
			(*it)->setDirectionY((*it)->getDirectionY()*-1);
		}
		
		if((*it)->getY() + (*it)->getHeight() > drawManager->getHeight()) {
			(*it)->setY((float)(drawManager->getHeight() - (*it)->getHeight()));			
			(*it)->setDirectionY((*it)->getDirectionY()*-1);
		}
	}
	
	// Update the balls location
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		(*it)->update((*it)->getSpeed() * deltatime);
	}

	// Manage balls vs bricks collision
	checkBallsBrickCollision();

	// Manage balls vs paddle collision
	checkBallsPaddleCollision();

	score_text->setText("Score: " + Utils::int_to_string(player->getScore()));

	// Restart game
	if(this->keyboard->IsDownOnce(SDLK_r)) {
		next_state = "Game";
		return false;
	}

	return true;
}

void Game::Draw() {
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

	life_text->draw();
	score_text->draw();
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
		for(unsigned int i = 0; i < row.length(); i++) {
			std::map<char, std::string>::iterator it = brick_info.find(row[i]);
			if(it == brick_info.end()) continue;
			Brick *brick = new Brick(it->second, (float)x, (float)y, brick_width, brick_height);
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

void Game::spawnBall() {
	Ball* ball = new Ball("ball.png", (float)(drawManager->getWidth() * 0.5) - 10, (float)(drawManager->getHeight() - 50 - 20), 20, 20);
	ball->Create(spriteManager, 0, 0);
	ball->setMaxSpeed((Config::defined("ball_max_speed") ? Utils::string_to_float(Config::get("ball_max_speed")) : 30.0));
	ball->setSpeed((Config::defined("ball_default_speed") ? Utils::string_to_float(Config::get("ball_default_speed")) : 15.0));
	float x_direction = ((Config::defined("start_balls_widex") ? Utils::string_to_float(Config::get("start_balls_widex")) : 8.0f));
	ball->setDirection(Utils::Random::frandom(-x_direction, x_direction), -1.0f);
	ball->freeze();
	balls.push_back(ball);
}

void Game::checkBallsBrickCollision() {
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		for(auto b = bricks.begin(); b != bricks.end(); ++b) {
			if(CollisionManager::collideRect((*it)->getBounds(), (*b)->getBounds())) {
				Brick *brick = (*b);
				Ball *ball = (*it);

				float collisionLeft = (ball->getX() + ball->getWidth()) - brick->getX();
				float collisionRight = ball->getX() - (brick->getX() + brick->getWidth());
				float collisionTop = (ball->getY() + ball->getHeight()) - brick->getY();
				float collisionBottom = ball->getY() - (brick->getY() + brick->getHeight());

				float cutoff = 1.0f;
				float bounced = false;

				if(fabsf(collisionLeft) < cutoff) {
					if(ball->getDirectionX() > 0.0f) {
						bounced = true;
						ball->invertDirectionX();
					}
				} else if(fabsf(collisionRight) < cutoff) {
					if(ball->getDirectionX() < 0.0f) {
						bounced = true;
						ball->invertDirectionX();
					}
				} else if(fabsf(collisionTop) < cutoff) {
					if(ball->getDirectionY() > 0.0f) {
						bounced = true;		
						ball->invertDirectionY();
					}
				} else if(fabsf(collisionBottom) < cutoff) {
					if(ball->getDirectionY() < 0.0f) {
						bounced = true;
						ball->invertDirectionY();
					}
				}
				if(bounced) {
					delete (*b);
					b = bricks.erase(b);
					--b;
				}
			}
		}
	}
}

void Game::checkBallsPaddleCollision() {
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		Ball *ball = (*it);
		std::vector<float> overflow(2, 0);
		if(CollisionManager::collideRectPlus((*it)->getBounds(), player->getBounds(), overflow)) {
            if(overflow[0] != 0 || overflow[1] != 0) {
				ball->increaseSpeed((Config::defined("paddle_bnc_speed_inc")) ? Utils::string_to_float(Config::get("paddle_bnc_speed_inc")) : 0.01);
				ball->setY(player->getY() - ball->getHeight());
				(*it)->setDirectionY((*it)->getDirectionY()*-1);				
				(*it)->setDirectionX(((((*it)->getX() + (*it)->getWidth()*.5) - (player->getX() + player->getWidth()*.5))/player->getWidth()*.5) * (Config::defined("paddle_bnc_wide") ? Utils::string_to_float(Config::get("paddle_bnc_wide")) : 3.0f));
            }
        }
	}
}