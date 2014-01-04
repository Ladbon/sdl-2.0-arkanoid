#pragma once

#include "State.h"
#include <list>
#include <vector>

class Player;
class Ball;
class Particle;
class Brick;
class Text;

class Game: public State
{
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

	void spawnBall();
	void checkBallsBrickCollision();
	void checkBallsPaddleCollision();

private:
	std::string next_state;
	int level;

	Player *player;
	std::vector<Ball*> balls;
	std::vector<Brick*> bricks;
	int rows;
	int columns;

	Ball* ball1;
	Ball* ball2;

	Text* life_text;
	Text* score_text;
};

