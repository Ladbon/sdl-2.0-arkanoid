#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "SDL.h"
#include "Utils.h"
#include "Game.h"
#include "DrawManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "Player.h"
#include "Brick.h"
#include "Ball.h"
#include "Config.h"
#include "Text.h"
#include "Color.h"
#include "Sound.h"

struct Rect {
	float top, bottom, left, right;
};

Game::Game() {
	level = 1;
	player = nullptr;
	balls.clear();
	life_text = nullptr;
	score_text = nullptr;
	gamearea_x = Config::getInt("gamearea_x", 0);
	gamearea_y = Config::getInt("gamearea_y", 0);
	paused = false;
};

bool Game::Init() {
	SoundManager::music("only_time")->play();

	SDL_WarpMouseInWindow(window, drawManager->getWidth()/2, drawManager->getHeight()/2);

	InitBricks();
	
	player = new Player("paddle_normal.png", (float)(drawManager->getWidth() * 0.5) - 50, (float)(drawManager->getHeight() - 50), 100, 20);
	player->Create(spriteManager, 0, 0);

	int start_balls = Config::getInt("start_balls_amount", 1);
	for(int i = 0; i < start_balls; i++) {
		spawnBall();
	}
	
	life_text = new Text(10, 10, "face_your_fears", 30, "Lifes: " + Utils::int_to_string(player->getLifes()), drawManager->getRenderer());
	score_text = new Text(drawManager->getWidth() - 200, 10, "face_your_fears", 30, "Score: 0", drawManager->getRenderer());

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
	// Pause the game
	if(InputManager::pressedWithoutRepeat(KEY_P)) {
		paused = !paused;
	}

	if(paused)  {
		return true;
	}
	if(InputManager::mousePressedOnce(MOUSE_LEFT)) {
		for(auto it = balls.begin(); it != balls.end(); ++it) {
			(*it)->unfreeze();
		}
	}

	// Player movement
	player->setX((float)(InputManager::mouseX-(player->getWidth()*0.5f)));
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		if((*it)->isFreezed()) {
			(*it)->setX((float)(InputManager::mouseX - (*it)->getWidth()*0.5f));
		}
	}

	// Player collision with bounds
	if(player->getX() + player->getWidth() > this->drawManager->getWidth()) { 
		player->setX((float)(this->drawManager->getWidth() - player->getWidth())); 
	}
	if(player->getY() + player->getHeight() > this->drawManager->getHeight()) { 
		player->setY((float)(this->drawManager->getHeight() - player->getHeight())); 
	}

	// Ball collision with bounds
	for(auto it = balls.begin(); it != balls.end(); ++it) {
		if((*it)->getX() < 0) {
			(*it)->setX(0.0f);
			(*it)->setDirectionX((*it)->getDirectionX()*-1);
		}

		if((*it)->getX() + (*it)->getWidth() > drawManager->getWidth()) {
			(*it)->setX((float)(drawManager->getWidth() - (*it)->getWidth()));
			(*it)->setDirectionX((*it)->getDirectionX()*-1);
		}
		
		if((*it)->getY() < (float)gamearea_y) {
			(*it)->setY((float)gamearea_y);			
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

	// Update score text
	score_text->setText("Score: " + Utils::int_to_string(player->getScore()));
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
	if(paused) drawPauseScreen();
}

bool Game::InitBricks() {
	brick_info.clear();
	// LOAD BRICK INFO
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
		brick_info.insert(std::pair<int, std::string>(Utils::char_to_int(id), filename));
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
			if(row[i] == '0') {
				x += brick_width;
				continue;
			}
			auto it = brick_info.find(Utils::char_to_int(row[i]));
			if(it == brick_info.end()) continue;
			Brick *brick = new Brick(it->second, (float)x, (float)y + (float)gamearea_y, brick_width, brick_height);
			brick->Create(spriteManager, 0, 0);
			brick->setLevel(Utils::char_to_int(row[i]));
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

void Game::drawPauseScreen() {
	
}

void Game::spawnBall() {
	Ball* ball = new Ball("ball.png", (float)(drawManager->getWidth() * 0.5) - 10, (float)(drawManager->getHeight() - 50 - 20), 20, 20);
	ball->Create(spriteManager, 0, 0);
	ball->setMaxSpeed(Config::getFloat("ball_max_speed", 30.0f));
	ball->setSpeed(Config::getFloat("ball_default_speed", 15.0f));
	float x_direction = Config::getFloat("start_balls_widex", 8.0f);
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
					if((*b)->onHit(1)) {
						player->appendScore((*b)->getScore());
						SoundManager::get("block_hit")->play();
						auto biit = brick_info.find((*b)->getLevel());
						(*b)->setFilename(biit->second);
						(*b)->Create(spriteManager, 0, 0);
					} else {
						player->appendScore((*b)->getScore() + Config::getInt("brick_destroy_bonus", 0));
						SoundManager::get("block_destroy")->play();
						delete (*b);
						b = bricks.erase(b);
						if(bricks.size() != 0)
							--b;
					}
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
				SoundManager::get("paddle_bounce")->play();
				ball->increaseSpeed(Config::getFloat("paddle_bnc_speed_inc", 0.01f));
				ball->setY(player->getY() - ball->getHeight());
				(*it)->setDirectionY((*it)->getDirectionY()*-1);				
				(*it)->setDirectionX(((((*it)->getX() + (*it)->getWidth()*.5f) - (player->getX() + player->getWidth()*.5f))/player->getWidth()*.5f) * Config::getFloat("paddle_bnc_wide", 3.0f));
            }
        }
	}
}