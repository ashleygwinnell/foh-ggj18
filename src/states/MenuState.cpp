
#include "MenuState.h"
#include "../DefaultGame.h"

#include <ARK2D/Core/State/Transition/FadeToColourTransition.h>
#include <ARK2D/Core/State/Transition/FadeFromColourTransition.h>

MenuState::MenuState():
	GameState() {

}

unsigned int MenuState::id() {
	return StatesList::STATE_MENU;
}

void MenuState::enter(GameContainer* container, StateBasedGame* game, GameState* from) {

}

void MenuState::leave(GameContainer* container, StateBasedGame* game, GameState* to) {

}

void MenuState::init(GameContainer* container, StateBasedGame* game) {
	DefaultGame* dg = DefaultGame::getInstance();
	m_selectedIndex = 0;

	logo = SpriteSheetStore::getImage("sprites/titlescreen-fizz.png");
	logoText = SpriteSheetStore::getImage("sprites/titlescreen-logo.png");

	timer2 = 0.0f;
}

void MenuState::update(GameContainer* container, StateBasedGame* game, GameTimer* timer) {
	DefaultGame* dg = DefaultGame::getInstance();
	Input* i = ARK2D::getInput();

	if (i->isKeyPressed(Input::KEY_ENTER) ||
		i->isGamepadButtonPressed(Gamepad::BUTTON_START) ||
		i->isGamepadButtonPressed(Gamepad::BUTTON_A)) {
	
		dg->stateInGame->reset();
		dg->stateInGame->start();
		//game->enterState(dg->stateInGame, new Fade);
		game->enterState(dg->stateInGame, 
			new FadeToColourTransition(0.4f, dg->bgColor),
			new FadeFromColourTransition(0.4f, dg->bgColor)
		);

		dg->m_sfxSelectStart->play();
	}

	timer2 += timer->getDelta();

	this->timer += timer->getDelta();
	if (this->timer > 2.0f) {
		this->timer = 0.0f;
	}

	/*if (i->isKeyPressed(Input::KEY_W) ||
		i->isKeyPressed(Input::KEY_UP) ||
		i->isGamepadButtonPressed(Gamepad::DPAD_UP)) {
		m_selectedIndex--;
		if (m_selectedIndex < MENU_BEGIN) {
			m_selectedIndex = MENU_END;
		}
	}
	if (i->isKeyPressed(Input::KEY_S) ||
		i->isKeyPressed(Input::KEY_DOWN) ||
		i->isGamepadButtonPressed(Gamepad::DPAD_DOWN)) {
		m_selectedIndex++;
		if (m_selectedIndex > MENU_END) {
			m_selectedIndex = MENU_BEGIN;
		}
	}


	if (i->isKeyPressed(Input::KEY_ENTER) ||
		i->isGamepadButtonPressed(Gamepad::BUTTON_START) ||
		i->isGamepadButtonPressed(Gamepad::BUTTON_A)) {
		if (m_selectedIndex == MENU_PLAY) {
			dg->stateInGame->reset();
			dg->stateInGame->start();
			game->enterState(dg->stateInGame);

		}
		else if (m_selectedIndex == MENU_INSTRUCTIONS) {
			game->enterState(dg->stateInstructions);
		}
		else if (m_selectedIndex == MENU_EXIT) {
			container->close();
		}
	}*/

	InGameState* igs = dg->stateInGame;
	igs->updateBackground(container, game, timer);
}

void MenuState::render(GameContainer* container, StateBasedGame* game, Renderer* r) {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = dg->stateInGame;

	igs->renderBackground(container, game, r);

	//r->drawString("Menu", 20, 20, Renderer::ALIGN_LEFT, Renderer::ALIGN_TOP);

	r->setDrawColor(Color::white);

	float addy = sin(timer2) * 3;
	float addy2 = sin(timer2+0.5f) * 3;

	logo->drawCentered(container->getWidth()*0.5f, (container->getHeight()*0.42f)+addy);
	logoText->drawCentered(container->getWidth()*0.5f, (container->getHeight()*0.62f)+addy2);


	if (this->timer < 1.33f) { 
		string str = "PRESS START!";
		r->setDrawColor(Color::black);
		r->drawString(str, (container->getWidth()*0.5f)+1, (container->getHeight() * 0.9f)+1, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);
		r->setDrawColor(Color::white);
		r->drawString(str, container->getWidth()*0.5f, (container->getHeight() * 0.9f), Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);
	}

	string str = "(C) @forcehabit + @2dforever + @SpicyJAudio";
	r->setFont(r->getDefaultFont());
	r->setDrawColor(Color::black);
	r->drawString(str, (container->getWidth()*0.5f)+1, (container->getHeight() - 6)+1, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);
	r->setDrawColor(Color::white);
	r->drawString(str, container->getWidth()*0.5f, container->getHeight() - 6, Renderer::ALIGN_CENTER, Renderer::ALIGN_BOTTOM);

	r->setFont(dg->font);
	// int rooty = 320;
	// int wid = (signed int) container->getWidth();

	// if (m_selectedIndex == MENU_PLAY) {
	// 	r->getFont()->drawStringCenteredAt("< Play >", wid/2, rooty);
	// }
	// else {
	// 	r->getFont()->drawStringCenteredAt("Play", wid/2, rooty);
	// }

	// if (m_selectedIndex == MENU_INSTRUCTIONS) {
	// 	r->getFont()->drawStringCenteredAt("< Instructions >", wid/2, rooty+40);
	// }
	// else {
	// 	r->getFont()->drawStringCenteredAt("Instructions", wid/2, rooty+40);
	// }

	// if (m_selectedIndex == MENU_EXIT) {
	// 	r->getFont()->drawStringCenteredAt("< Exit >", wid/2, rooty+80);
	// }
	// else {
	// 	r->getFont()->drawStringCenteredAt("Exit", wid/2, rooty+80);
	// }
}

MenuState::~MenuState() {

}
