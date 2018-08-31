#include "ScrollObject.h"
#include "../DefaultGame.h"

ScrollObject::ScrollObject():
	GameObject() {
		percentageX = 0;
		percentageY = 0;
		sizex = 40;
		sizey = 40;
		m_bounds  = new ARK::Core::Geometry::Rectangle(0, 0, sizex, sizey);
		shadowy = 0;

		startx = 0;
		starty = 0;
		endx = 0;
		endy = 0;
		actualx = 0;
		actualy = 0;
		speed = 0;
		m_stopAt = 0;
}
ScrollObject::~ScrollObject() {
    
}
void ScrollObject::update(GameContainer* container, GameTimer* timer) {
	
    InGameState* igs = DefaultGame::getInstance()->stateInGame;
	z -= speed * igs->speedMultiplier * timer->getDelta();
	recalcPositions();

	if ((speed > 0 && z < 0.5f) || (speed < 0 && z > igs->m_farDistance)) {
		setPendingRemoval(true);
	}
}
void ScrollObject::recalcPositions() {
	InGameState* igs = DefaultGame::getInstance()->stateInGame;

	startx = igs->m_farBounds->getMinX() + (percentageX * igs->m_farBounds->getWidth());
	starty = igs->m_farBounds->getMinY() + (percentageY * igs->m_farBounds->getHeight());

	//endx = m_nearBounds->getMinX() + (((s->startx - m_farBounds->getMinX()) / m_farBounds->getWidth()) * m_nearBounds->getWidth());
	//endy = m_nearBounds->getMinY() + (((s->starty - m_farBounds->getMinY()) / m_farBounds->getHeight()) * m_nearBounds->getHeight());

	endx = igs->m_nearBounds->getMinX() + (percentageX * igs->m_nearBounds->getWidth()); 
	endy = igs->m_nearBounds->getMinY() + (percentageY * igs->m_nearBounds->getHeight());

	actualx = Easing::easebetween(Easing::QUARTIC_IN, igs->m_farDistance - z, startx, endx, igs->m_farDistance,true);
	actualy = Easing::easebetween(Easing::QUARTIC_IN, igs->m_farDistance - z, starty, endy, igs->m_farDistance,true);
	// float usez = z;
	// if (z > 0) {
	// 	usez = 0;
	// }
	float actualScale = Easing::easebetween(Easing::CIRCLE_IN, igs->m_farDistance - z, igs->m_farScale, 1.0f, igs->m_farDistance,false);
	if (z < 0) {
		actualScale = 1;
		// float endx1 = (2*igs->m_nearBounds->getMinX()) + (2*percentageX * igs->m_nearBounds->getWidth()); 
		// float endy1 = (2*igs->m_nearBounds->getMinY()) + (2*percentageY * igs->m_nearBounds->getHeight());

		// actualScale = Easing::easebetween(Easing::CIRCLE_OUT, abs(z), igs->m_farScale, 2.0f, igs->m_farDistance, true);
		// actualx = Easing::easebetween(Easing::QUARTIC_OUT, abs(z), endx, endx1, igs->m_farDistance,true);
		// actualy = Easing::easebetween(Easing::QUARTIC_OUT, abs(z), endy, endy1, igs->m_farDistance,true);
	}
	m_bounds->asRectangle()->setSize(sizex * actualScale, sizey * actualScale);
	m_bounds->setLocationByAlignment(actualx, actualy, 0, 1);

	float shadowstarty = igs->m_farBounds->getMinY() + (1.0f * igs->m_farBounds->getHeight());
	float shadowendy = igs->m_nearBounds->getMinY() + (1.0f * igs->m_nearBounds->getHeight());
	shadowy = Easing::easebetween(Easing::QUARTIC_IN, igs->m_farDistance - z, shadowstarty, shadowendy, igs->m_farDistance,false);

}
void ScrollObject::render(GameContainer* container, Renderer* r) {
	
    InGameState* igs = DefaultGame::getInstance()->stateInGame;
	float actualScale = Easing::easebetween(Easing::CIRCLE_IN, igs->m_farDistance - z, igs->m_farScale, 1.0f, igs->m_farDistance,false);
	float szw = sizex * actualScale;
	float szh = sizey * actualScale;
	r->fillRect(actualx - (szw*0.5), actualy - szh, szw, szh);

	renderBounds(container, r);
}
void ScrollObject::renderBounds(GameContainer* container, Renderer* r) {

	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	
	r->setDrawColor(Color::red);
	r->drawRect(m_bounds->getMinX(), m_bounds->getMinY(), m_bounds->getWidth(), m_bounds->getHeight());

}
void ScrollObject::renderImage(GameContainer* container, Renderer* r, Image* img, float offsety, float offsetx) {
	InGameState* igs = DefaultGame::getInstance()->stateInGame;
	float actualScale = Easing::easebetween(Easing::CIRCLE_IN, igs->m_farDistance - z + m_stopAt, igs->m_farScale, 1.0f, igs->m_farDistance,false);
	if (z < 0) {
		//actualScale = Easing::easebetween(Easing::CIRCLE_OUT, abs(z), igs->m_farScale, 2.0f, 10, true);
		actualScale = 1;
	}

	//float szw = sizex * actualScale;
	m_bounds->asRectangle()->setSize(sizex * actualScale, sizey * actualScale);
	m_bounds->setLocationByAlignment(actualx, actualy, 0, 1);
	img->drawAligned(actualx, actualy+(offsety*igs->m_nearBounds->getHeight()*actualScale), 0, 1, actualScale);
}


