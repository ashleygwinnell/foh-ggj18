#include "Explosion.h"
#include "../DefaultGame.h"

AnimationFrameList* Explosion::s_explosion1 = NULL;
AnimationFrameList* Explosion::s_explosion2 = NULL;

Explosion::Explosion():
	ScrollObject() 
	{
        DefaultGame* dg = DefaultGame::getInstance();
	if (s_explosion1 == NULL) {
		s_explosion1 = AnimationFrameList::create("sprites/effects/explosion1-*.png", 0, 18, dg->spritesheet, dg->desc, 2);
		s_explosion2 = AnimationFrameList::create("sprites/effects/explosion2-*.png", 0, 19, dg->spritesheet, dg->desc, 2);
	}
	m_animation = new Animation();
}
void Explosion::reset() {
	m_animation->clear();
	m_animation->reset();
}
void Explosion::update(GameContainer* container, GameTimer* timer) {
	ScrollObject::update(container, timer);
	setPendingRemoval(false);

	signed int i = m_animation->getCurrentFrameIndex(); 
	m_animation->update(timer);
	if (m_animation->getCurrentFrameIndex() < i) {
		setPendingRemoval(true);
	}
}
void Explosion::render(GameContainer* container, Renderer* r) {
	r->setDrawColor(Color::white);
	//ScrollObject::render(container, r);
	renderImage(container, r, m_animation->getCurrentFrame());
}
