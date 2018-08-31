
#ifndef ARK2D_DEFAULTGAME_SPLASHSTATE_H_
#define ARK2D_DEFAULTGAME_SPLASHSTATE_H_

#include <ARK2D/Core/State/GameState.h>

class DefaultGame;

class SplashState : public GameState {

	public:
		SplashState();
		void enter(GameContainer* container, StateBasedGame* game, GameState* from);
		void leave(GameContainer* container, StateBasedGame* game, GameState* to);

		unsigned int id();
		void init(GameContainer* container, StateBasedGame* game);
		void update(GameContainer* container, StateBasedGame* game, GameTimer* timer);
		void render(GameContainer* container, StateBasedGame* game, Renderer* r);

		virtual ~SplashState();
};

#endif /* ARK2D_DEFAULTGAME_SPLASHSTATE_H_ */
