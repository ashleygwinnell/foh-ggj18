/*
 * Object.cpp
 *
 *  Created on: 18 April 2012
 *      Author: Ashley
 */

#include "Object.h"
//#include "MyParticle.h"
#include "../DefaultGame.h"
#include <ARK2D/Core/+.h>

float Object::s_gravity = 800.0f; 

Object::Object():
	GameObject(),
	m_bounds(NULL)
	{
	
	m_velocity = new Vector2<float>(0,0);
	m_velocityMax = new Vector2<float>(0,0);
	m_acceleration = new Vector2<float>(0,0);
}

void Object::update(GameContainer* container, GameTimer* timer) {
	GameObject::update();
}


void Object::reset() {
	components.clear();
}

void Object::render(GameContainer* container, Renderer* r) {
	//if (ARK2D::isDebug() && m_bounds != NULL) {
	//	m_bounds->render();
	//}
	GameObject::render();
	//if (DefaultGame::getInstance()->isShowingDebugBounds() && m_bounds != NULL) {
	//	m_bounds->render();
	//}

}


void Object::fireParticles() {
    DefaultGame* dg = DefaultGame::getInstance();
    InGameState* igs = dg->stateInGame;
/*
	MyParticle* part = NULL;
	
	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud55);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-135.0f, -45.0f), MathUtil::randBetweenf(10.0f, 30.0f));

	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud33);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-135.0f, -45.0f), MathUtil::randBetweenf(10.0f, 30.0f));

	part = igs->m_particles->get();
	part->reset();
	part->m_animation->addFrame(MyParticle::s_cloud22);
	part->m_duration = MathUtil::randBetweenf(1.0f, 2.0f);
	part->m_type = MyParticle::TYPE_SMOKE;
	part->m_bounds->setLocationByCenter(m_bounds->getCenterX(), m_bounds->getCenterY());
	part->m_velocity->set(0.0f, 0.0f);
	MathUtil::moveAngle<float>(part->m_velocity, MathUtil::randBetweenf(-135.0f, -45.0f), MathUtil::randBetweenf(10.0f, 30.0f));
	*/
}

void Object::move(float multiplier = 1) {
	GameTimer* timer = ARK2D::getContainer()->getTimer();
    ARK::Core::Geometry::Rectangle* bounds = m_bounds->asRectangle();
	bounds->adjustX(m_velocity->getX() * multiplier * timer->getDelta());
	bounds->adjustY(m_velocity->getY() * multiplier * timer->getDelta());
}

void Object::gravity(float multiplier) {
	GameTimer* timer = ARK2D::getContainer()->getTimer();
	
	m_velocity->add(0, s_gravity * multiplier * timer->getDelta()); 
	
//    if (m_restrictVelocity) { 
//        m_velocity->restrictMax(m_velocityMax);
//    }
}

void Object::collision(Object* other) {

}

Object::~Object() {
	delete m_velocity;
	delete m_velocityMax;
}
