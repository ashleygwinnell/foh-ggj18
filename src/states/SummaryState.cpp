
#include "SummaryState.h"
#include "../DefaultGame.h"

#include <ARK2D/Core/State/Transition/FadeToColourTransition.h>
#include <ARK2D/Core/State/Transition/FadeFromColourTransition.h>

SummaryState::SummaryState():
    GameState() {

}

unsigned int SummaryState::id() {
	return StatesList::STATE_SUMMARY;
}

void SummaryState::enter(GameContainer* container, StateBasedGame* game, GameState* from) { 
	DefaultGame::getInstance()->m_musicGameover->play();
}
void SummaryState::leave(GameContainer* container, StateBasedGame* game, GameState* to) { }

void SummaryState::init(GameContainer* container, StateBasedGame* game) {
	DefaultGame* dg = DefaultGame::getInstance();

}

void SummaryState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	
	DefaultGame* dg = DefaultGame::getInstance();
	Input* in = ARK2D::getInput();
	if (in->isKeyPressed(Input::KEY_ENTER) ||
        in->isGamepadButtonPressed(Gamepad::BUTTON_START) ||
        in->isGamepadButtonPressed(Gamepad::BUTTON_A)) {
		//dg->enterState(dg->stateMenu);
		 game->enterState(dg->stateMenu, 
				new FadeToColourTransition(0.4f, dg->bgColor),
				new FadeFromColourTransition(0.4f, dg->bgColor)
			);

	}

	InGameState* igs = dg->stateInGame;
	//igs->updateBackground(container, game, timer);
}  

void SummaryState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
	
	DefaultGame* dg = DefaultGame::getInstance();
	r->setDrawColor(Color::white);
	//r->drawString("Summary State", 20, 20, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

	r->setDrawColor(Color::white);
	
    InGameState* igs = DefaultGame::getInstance()->stateInGame;
	//igs->render(container, game, r);
    igs->renderBackground(container, game, r);

	string str = "OH, THAT STINGS!";
	r->setDrawColor(Color::black);
	r->drawString(str, (container->getWidth()*0.5f)+1, (container->getHeight() * 0.4f)+1, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM, 0.0f, 2.0f);
	r->setDrawColor(Color::white);
	r->drawString(str, container->getWidth()*0.5f, (container->getHeight() * 0.4f), Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM, 0.0f, 2.0f);

	str = StringUtil::append("SCORE: ", igs->m_pollenCount);
	r->setDrawColor(Color::black);
	r->drawString(str, (container->getWidth()*0.5f)+1, (container->getHeight() * 0.6f)+1, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM, 0.0f, 3.0f);
	r->setDrawColor(Color::white);
	r->drawString(str, container->getWidth()*0.5f, (container->getHeight() * 0.6f), Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM, 0.0f, 3.0f);

	str = "(C) @forcehabit + @2dforever + @SpicyJAudio";
	r->setFont(r->getDefaultFont());
	r->setDrawColor(Color::black);
	r->drawString(str, (container->getWidth()*0.5f)+1, (container->getHeight() - 6)+1, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);
	r->setDrawColor(Color::white);
	r->drawString(str, container->getWidth()*0.5f, container->getHeight() - 6, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);
	r->setFont(dg->font);
}

SummaryState::~SummaryState() {

}

