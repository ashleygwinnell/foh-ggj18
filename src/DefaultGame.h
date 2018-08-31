
#ifndef ARK2D_DEFAULTGAME_H_
#define ARK2D_DEFAULTGAME_H_

#include <ARK2D/Audio/Sound.h>
#include <ARK2D/Core/Graphics/Image.h>
#include <ARK2D/Core/Graphics/SpriteSheetDescription.h>
#include <ARK2D/Core/State/StateBasedGame.h>
#include <ARK2D/Core/+.h>
#include <ARK2D/Audio/SoundStore.h>

#include "states/BlankState.h"
#include "states/InGameState.h"
#include "states/InstructionsState.h"
#include "states/MenuState.h"
#include "states/SplashState.h"
#include "states/SummaryState.h"

class StatesList {
	public:
		static const unsigned int STATE_BLANK = 0;
		static const unsigned int STATE_LOADING = 1;
		static const unsigned int STATE_SPLASH = 2;
		static const unsigned int STATE_MENU = 3;
		static const unsigned int STATE_INSTRUCTIONS = 4;
		static const unsigned int STATE_INGAME = 5;
		static const unsigned int STATE_SUMMARY = 6;
};

class DefaultGame : public StateBasedGame {
	public:

		BlankState* stateBlank = NULL;
		SplashState* stateSplash = NULL;
		MenuState* stateMenu = NULL;
		InstructionsState* stateInstructions = NULL;
		InGameState* stateInGame = NULL;
		SummaryState* stateSummary = NULL;

		Image* spritesheet = NULL;
		SpriteSheetDescription* desc = NULL;

		Sound* m_music = NULL;
		Sound* m_musicMenu = NULL;
		Sound* m_musicGame = NULL;
		Sound* m_musicGameover = NULL;

		Sound* m_sfxTest = NULL;

		vector<Sound*>* m_sfxBeePickup = NULL;
		Sound* m_sfxBeePowerupBig = NULL;
		Sound* m_sfxBeePowerupSmall = NULL;
		Sound* m_sfxBeeShoot = NULL;
		Sound* m_sfxBeeSplatCrow = NULL;
		Sound* m_sfxBeeSplatRock = NULL;
		Sound* m_sfxBushRustle = NULL;
		Sound* m_sfxCrowAppear = NULL;
		Sound* m_sfxCrowShoot = NULL;
		Sound* m_sfxExplosion = NULL;
        vector<Sound*>* m_sfxSelectChange = NULL;
		Sound* m_sfxSelectLocked = NULL;
		Sound* m_sfxSelectPress = NULL;
		Sound* m_sfxSelectStart = NULL;
		Sound* m_sfxShieldDown = NULL;

		vector<Sound*>* m_voEating = NULL;
		vector<Sound*>* m_voPowerup = NULL;
		vector<Sound*>* m_voDie = NULL;
		vector<Sound*>* m_voStart = NULL;
		
		FBO* m_renderTarget = NULL;
		Color* bgColor = NULL;

		ARK::Core::Font::BMFont* font = NULL;

		bool m_debug = false;; 

		DefaultGame(string title);
		virtual void initStates(GameContainer* container);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void render();
		virtual void resize(GameContainer* container, int width, int height);
		virtual ~DefaultGame();

		static DefaultGame* getInstance();
};

#endif /* ARK2D_DEFAULTGAME_H_ */
