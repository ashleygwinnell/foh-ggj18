
#include "SplashState.h"
#include "../DefaultGame.h"

SplashState::SplashState():
	GameState() {

}

unsigned int SplashState::id() {
	return StatesList::STATE_BLANK;
}

void SplashState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}

void SplashState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void SplashState::init(GameContainer* container, StateBasedGame* game) {

}

void SplashState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {

}

void SplashState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {

}

SplashState::~SplashState() {

}
