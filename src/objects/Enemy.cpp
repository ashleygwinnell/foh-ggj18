#include "Enemy.h"
#include "../DefaultGame.h"

AnimationFrameList* Enemy::s_animCrow = NULL;
AnimationFrameList* Enemy::s_animCrowAttack = NULL;

Enemy::Enemy():
	ScrollObject() 
	{
    DefaultGame* dg = DefaultGame::getInstance();
	if (s_animCrow == NULL) {
		s_animCrow = AnimationFrameList::create("sprites/enemy/crow*.png", 0, 5, dg->spritesheet, dg->desc, 2);
		s_animCrowAttack = AnimationFrameList::create("sprites/enemy/crow-attack*.png", 0, 5, dg->spritesheet, dg->desc, 2);
	}
	m_animation = new Animation();
	reset();
}
void Enemy::reset() {
	m_type = TYPE_CROW;
	m_animation->clear();
	m_stopAt = 10;

	m_fireTimer = 0.0f;
	m_fireDuration = 3.0f;

	
	fireAnim = false;
	fireAnimFired = false;

}
void Enemy::die() {
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	igs->startExplosion(percentageX, percentageY, z, speed);
	igs->m_cameraShake->setSpeedMultiplier(1, 0.75f);
	igs->m_cameraShake->start(1, 5.0f);

	setPendingRemoval(true);
	// todo die sound();
}

void Enemy::start(int type) {
	if (type == TYPE_CROW) {
		m_animation->setFrameTime(0.12f);
		m_animation->setFrames(s_animCrow);
	}
}
void Enemy::update(GameContainer* container, GameTimer* timer) {
	ScrollObject::update(container, timer);
	

	if (z < m_stopAt) {
		z = m_stopAt;
	}

	if (z <= m_stopAt) {
		m_fireTimer += timer->getDelta();
		if (m_fireTimer >= m_fireDuration) {

			m_animation->setFrameTime(0.12f);
			m_animation->setFrames(s_animCrowAttack);
			fireAnim = true;
			fireAnimFired = false;
			

			m_fireTimer -= m_fireDuration;;

			
		}
	}

	signed int i = m_animation->getCurrentFrameIndex(); 
	m_animation->update(timer);

	if (fireAnim) {
		if (m_animation->getCurrentFrameIndex() >= 3 && !fireAnimFired) {
			// pow
            InGameState* igs = DefaultGame::getInstance()->stateInGame;
			Bullet* b = igs->m_bullets->get();
			b->player = false;
			b->reset();
			b->percentageX = percentageX;
			b->percentageY = percentageY;
			b->recalcPositions();
			b->z = z;
			b->sizex = 40;
    		b->sizey = 40;

			b->speed = InGameState::generalSpeed * 0.2;

			DefaultGame* dg = DefaultGame::getInstance();
			dg->m_sfxCrowShoot->play();

			fireAnimFired = true;

			//dg->m_sfxBeeShoot->play();
		}
		if (m_animation->getCurrentFrameIndex() < i) {
			m_animation->setFrameTime(0.12f);
			m_animation->setFrames(s_animCrow);
			m_animation->setCurrentFrameIndex(3);
		}
	}
}
void Enemy::render(GameContainer* container, Renderer* r) {
	r->setDrawColor(Color::white);
	renderImage(container, r, m_animation->getCurrentFrame(), 0.13f);
	//renderBounds(container, r);

	//return; 

	/*InGameState* igs = DefaultGame::getInstance()->stateInGame;
	float actualScale = Easing::easebetween(Easing::CIRCLE_IN, igs->m_farDistance - z + m_stopAt, igs->m_farScale, 1.0f, igs->m_farDistance,false);
	Image* img = igs->m_player->m_shadow;
	img->setAlpha(1.0f);
	img->drawAligned(actualx, actualy, 0, 1, actualScale);*/

	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	Image* img = igs->m_player->m_shadow;
	float actualScale = Easing::easebetween(Easing::CIRCLE_IN, igs->m_farDistance - z, igs->m_farScale, 1.0f, igs->m_farDistance,false);
	img->drawAligned(actualx, shadowy, 0, 1, actualScale);
}
