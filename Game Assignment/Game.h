#pragma once

#include "State.h"
#include <list>
#include <vector>
#include <map>

class Player;
class Ball;
class Particle;
class Brick;
class Text;

class Game: public State {
public:
	Game();
	bool Init();
	void Exit();

	bool Update();
	void Draw();

	std::string Next();
	void setNextState(std::string state);
	bool IsType(const std::string &type);
	bool LoadBrickInfo();
	bool InitBricks();

	// Game state specific methods
	void spawnBall();
	void checkBallsBrickCollision();
	void checkBallsPaddleCollision();
	void drawPauseScreen();

private:
	bool paused;
	int gamearea_x;
	int gamearea_y;
	std::string next_state;
	int level;

	Player *player;
	std::vector<Ball*> balls;

	std::map<int, std::string> brick_info;
	std::vector<Brick*> bricks;
	int rows;
	int columns;

	Text* life_text;
	Text* score_text;
};

