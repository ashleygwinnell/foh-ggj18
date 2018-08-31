
#include "Bullet.h"
#include "../DefaultGame.h"

AnimationFrameList* Bullet::s_animCenterTop = NULL;
AnimationFrameList* Bullet::s_animCenter = NULL;
AnimationFrameList* Bullet::s_animCenterBottom = NULL;

AnimationFrameList* Bullet::s_animSideTop = NULL;
AnimationFrameList* Bullet::s_animSide = NULL;
AnimationFrameList* Bullet::s_animSideBottom = NULL;

AnimationFrameList* Bullet::s_animSideRTop = NULL;
AnimationFrameList* Bullet::s_animSideR = NULL;
AnimationFrameList* Bullet::s_animSideRBottom = NULL;

AnimationFrameList* Bullet::s_tornado = NULL;

Bullet::Bullet():
	ScrollObject() {

        DefaultGame* dg = DefaultGame::getInstance();
		if (s_animCenterTop == NULL) {
			s_animCenterTop = AnimationFrameList::create("sprites/character/shoot-top-centre*.png", 1, 2, dg->spritesheet, dg->desc);
			s_animCenter = AnimationFrameList::create("sprites/character/shoot-centre*.png", 1, 2, dg->spritesheet, dg->desc);
			s_animCenterBottom = AnimationFrameList::create("sprites/character/shoot-bottom-centre*.png", 1, 2, dg->spritesheet, dg->desc);
			
			s_animSideTop = AnimationFrameList::create("sprites/character/shoot-top-side*.png", 1, 2, dg->spritesheet, dg->desc);
			s_animSide = AnimationFrameList::create("sprites/character/shoot-side*.png", 1, 2, dg->spritesheet, dg->desc);
			s_animSideBottom = AnimationFrameList::create("sprites/character/shoot-bottom-side*.png", 1, 2, dg->spritesheet, dg->desc);
			
			s_animSideRTop = AnimationFrameList::create("sprites/character/shoot-top-side*.png", 1, 2, dg->spritesheet, dg->desc);
			s_animSideR = AnimationFrameList::create("sprites/character/shoot-side*.png", 1, 2, dg->spritesheet, dg->desc);
			s_animSideRBottom = AnimationFrameList::create("sprites/character/shoot-bottom-side*.png", 1, 2, dg->spritesheet, dg->desc);
			
			s_tornado = AnimationFrameList::create("sprites/enemy/tornado*.png", 1, 4, dg->spritesheet, dg->desc);
			

			for (unsigned int i = 0; i < s_animSideRTop->getFrames()->size(); i++) {
				s_animSideRTop->getFrames()->at(i)->flip(true, false);
			}
			for (unsigned int i = 0; i < s_animSideR->getFrames()->size(); i++) {
				s_animSideR->getFrames()->at(i)->flip(true, false);
			}
			for (unsigned int i = 0; i < s_animSideRBottom->getFrames()->size(); i++) {
				s_animSideRBottom->getFrames()->at(i)->flip(true, false);
			}
			
		}

	m_animation = new Animation();
	m_animation->setFrameTime(0.1f);

	player = true;

	if (m_bounds) { delete m_bounds; m_bounds = NULL; }

	sizex = 20;
    sizey = 20;
	speed = InGameState::generalSpeed * -1;
	m_bounds  = new ARK::Core::Geometry::Rectangle(0, 0, sizex, sizey);
}
void Bullet::recalcPositions() {
	ScrollObject::recalcPositions();	
}
void Bullet::reset() {
	speed = InGameState::generalSpeed * -1;
	sizex = 20;
    sizey = 20;
	m_bounds->asRectangle()->setSize(sizex, sizey);

	m_animation->clear();
	m_animation->setFrameTime(0.025f);
	m_animation->addFrames(getAnimationForPosition());
}
AnimationFrameList* Bullet::getAnimationForPosition() {
	GameContainer* container = ARK2D::getContainer();
	InGameState* igs = DefaultGame::getInstance()->stateInGame;

	if (!player) {
		return s_tornado;
	}
	
    float percentageSide = (container->getWidth()*0.5f) / igs->m_nearBounds->getWidth();
	if (percentageX > percentageSide && percentageSide < percentageX > 1.0f - percentageSide && 
		percentageY > 0.3f && percentageY < 0.7f) {
		return s_animCenter;
	}

	return s_animSide;

	
	if (percentageY > 0.7) {
		if (percentageX < percentageSide) {
			return s_animSideBottom;
		}
		else if (percentageX > 1.0 - percentageSide) {
			return s_animSideRBottom;
		}
		return s_animCenterBottom;
	}
	else if (percentageY < 0.3) {
		if (percentageX < percentageSide) {
			return s_animSideTop;
		}
		else if (percentageX > 1.0 - percentageSide) {
			return s_animSideRTop;
		}
		return s_animCenterTop;
	}
	if (percentageX < percentageSide) {
		return s_animSide;
	}
	else if (percentageX > 1.0 - percentageSide) {
		return s_animSideR;
	}
	return s_animCenter;


}

void Bullet::update(GameContainer* container, GameTimer* timer) {
	ScrollObject::update(container, timer);
	m_animation->update(timer);
	

	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	if (!player) {
		setPendingRemoval(false);
		if (z < -2) {
			setPendingRemoval(true);
		}

		float angle = MathUtil::anglef(percentageX, percentageY, igs->m_player->getPercentageX(), igs->m_player->getPercentageY());
		MathUtil::moveAngle<float>(percentageX, percentageY, angle, 0.15 * timer->getDelta() );
	}
}
void Bullet::render(GameContainer* container, Renderer* r) {
	r->setDrawColor(Color::white);
	//ScrollObject::render(container, r);
	Image* img = m_animation->getCurrentFrame();
	if (getAnimationForPosition() == s_animSide) {
		img->setRotation(MathUtil::anglef(percentageX, percentageY, 0.5f, 0.5f));
	}
	renderImage(container, r, img);

	//r->setDrawColor(Color::black);
	//r->drawString(StringUtil::append("", z), m_bounds->getCenterX(), m_bounds->getCenterY());
}
