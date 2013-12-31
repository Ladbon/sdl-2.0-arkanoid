#include "Highscore.h"

Highscore::Highscore() {
	
};

bool Highscore::Init() {	
	printf("Highscore Init");
	return true;
}

void Highscore::Exit() {	
	printf("Highscore Exit");
}


bool Highscore::Update() {
	return true;
}

void Highscore::Draw() {

}

std::string Highscore::Next() {
	return next_state;
};


void Highscore::setNextState(std::string state) {
	next_state = state;
}

bool Highscore::IsType(const std::string &type) {
	return type.compare("Highscore") == 0;
};