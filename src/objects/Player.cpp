#include "Player.h"
#include "../DefaultGame.h"

Player::Player():
	Object(),
	additionalVelocity(0, 0) {
        DefaultGame* dg = DefaultGame::getInstance();
	m_bounds = new ARK::Core::Geometry::Circle<float>(0,0,15);

	m_movement = new EightWayMovementComponent();
	m_movement->setController(true, 0, Gamepad::ANALOG_STICK_1_X);
	m_movement->setKeysWASD();

	
	float frameTime = 0.1f; 
	m_animCenterTop = new Animation();
	m_animCenter = new Animation();
	m_animSideTop = new Animation();
	m_animSide = new Animation();
	m_animSideRTop = new Animation();
	m_animSideR = new Animation();
	m_animGround = new Animation();
	
	m_animDeath = new Animation();
	m_animDeathLoop = new Animation();

	m_animCenterTop->setFrameTime(frameTime);
	m_animCenter->setFrameTime(frameTime);
	m_animSideTop->setFrameTime(frameTime);
	m_animSide->setFrameTime(frameTime);
	m_animSideRTop->setFrameTime(frameTime);
	m_animSideR->setFrameTime(frameTime);
	m_animGround->setFrameTime(frameTime*0.7f);

	m_animDeath->setFrameTime(frameTime);
	m_animDeathLoop->setFrameTime(frameTime);

	m_animCenterTop->addFrame(SpriteSheetStore::getImage("sprites/character/top-centre-00.png"));
	m_animCenterTop->addFrame(SpriteSheetStore::getImage("sprites/character/top-centre-01.png"));
	
	m_animCenter->addFrame(SpriteSheetStore::getImage("sprites/character/centre-00.png"));
	m_animCenter->addFrame(SpriteSheetStore::getImage("sprites/character/centre-01.png"));

	m_animSideTop->addFrame(SpriteSheetStore::getImage("sprites/character/top-side-00.png"));
	m_animSideTop->addFrame(SpriteSheetStore::getImage("sprites/character/top-side-01.png"));

	m_animSideRTop->addFrame((Image*)SpriteSheetStore::getImage("sprites/character/top-side-00.png")->flip(true,false));
	m_animSideRTop->addFrame((Image*)SpriteSheetStore::getImage("sprites/character/top-side-01.png")->flip(true,false));

	m_animSide->addFrame(SpriteSheetStore::getImage("sprites/character/side-00.png"));
	m_animSide->addFrame(SpriteSheetStore::getImage("sprites/character/side-01.png"));

	m_animSideR->addFrame((Image*)SpriteSheetStore::getImage("sprites/character/side-00.png")->flip(true,false));
	m_animSideR->addFrame((Image*)SpriteSheetStore::getImage("sprites/character/side-01.png")->flip(true,false));

	m_animGround->addFrame(SpriteSheetStore::getImage("sprites/character/ground-00.png"));
	m_animGround->addFrame(SpriteSheetStore::getImage("sprites/character/ground-01.png"));
	m_animGround->addFrame(SpriteSheetStore::getImage("sprites/character/ground-02.png"));
	m_animGround->addFrame(SpriteSheetStore::getImage("sprites/character/ground-03.png"));
	m_animGround->addFrame(SpriteSheetStore::getImage("sprites/character/ground-04.png"));
	m_animGround->addFrame(SpriteSheetStore::getImage("sprites/character/ground-05.png"));

	m_animDeath->addFrame(SpriteSheetStore::getImage("sprites/character/death-00.png"));
	m_animDeath->addFrame(SpriteSheetStore::getImage("sprites/character/death-01.png"));
	m_animDeath->addFrame(SpriteSheetStore::getImage("sprites/character/death-02.png"));
	m_animDeathLoop->addFrame(SpriteSheetStore::getImage("sprites/character/death-03.png"));
	m_animDeathLoop->addFrame(SpriteSheetStore::getImage("sprites/character/death-04.png"));

	m_shadow = SpriteSheetStore::getImage("sprites/character/bee-shadow.png");

	m_shield = true;
	AnimationFrameList* shieldFrames = AnimationFrameList::create("sprites/character/shield-*.png", 0, 15, dg->spritesheet, dg->desc, 2);
	m_animShield = new Animation();
	m_animShield->setFrameTime(frameTime/4);
	m_animShield->addFrames(shieldFrames);

	reset();
}


void Player::reset() {
	speed = 0;
	z = 0;
	m_moving = false;

	m_hitTimer = 0.0f;
	m_hitDuration = 3.0f;

	additionalVelocity.set(0, 0);

	m_velocity->set(0, 0);
	m_velocityMax->set(175, 175);
	m_acceleration->set(0, 0);

	m_indeathloop = false;
	m_shield = true;

	m_fireTimer = 0.0f;
	m_fireDuration = 0.4f;
}

void Player::hit() {
	if (!canBeHit()) return; 

	if (m_shield) {
		m_hitTimer = 0.01f;

		m_shield = false;
		DefaultGame::getInstance()->m_sfxShieldDown->play();
	}
	else {
		die();
	}
}

void Player::die() {
	DefaultGame* dg = DefaultGame::getInstance();
	InGameState* igs = DefaultGame::getInstance()->stateInGame;

	igs->m_deathTimer = 0.01f;
	igs->m_spawnEnemies.clear();

	m_velocity->set(0.0f, -500.0f);
	m_velocityMax->set(875, 875);

	dg->m_voDie->at(MathUtil::randBetween(0, dg->m_voDie->size()))->play();
}

bool Player::canBeHit() {
	return m_hitTimer == 0.0f;
}


float Player::getPercentageX() {
	InGameState* igs = DefaultGame::getInstance()->stateInGame;;
	return (m_bounds->getCenterX() - igs->m_nearBounds->getMinX()) / igs->m_nearBounds->getWidth();
}
float Player::getPercentageY() {
	InGameState* igs = DefaultGame::getInstance()->stateInGame;;
	return m_bounds->getCenterY() / igs->m_nearBounds->getMaxY();
}

Animation* Player::getCurrentAnimation() {
    GameContainer* container = ARK2D::getContainer();
	InGameState* igs = DefaultGame::getInstance()->stateInGame;

	if (igs->m_deathTimer > 0.0f) {
		if (m_indeathloop) { 
			return m_animDeathLoop;
		} 
		return m_animDeath;
	}

	if (m_bounds->getCenterY() > igs->m_nearBounds->getMaxY()*0.9 && igs->m_barrelRollTimer == 0.0f) {
		return m_animGround;
	}
	if (m_bounds->getCenterY() > igs->m_nearBounds->getMaxY()*0.7) {
		if (m_bounds->getCenterX() < igs->m_nearBounds->getMinX() + (container->getWidth()*0.5f)) {
			return m_animSide;
		}
		else if (m_bounds->getCenterX() > igs->m_nearBounds->getMaxX() - (container->getWidth()*0.5f)) {
			return m_animSideR;
		}
		return m_animCenter;
	}

	if (m_bounds->getCenterX() < igs->m_nearBounds->getMinX() + (container->getWidth()*0.5f)) {
		return m_animSideTop;
	}
	else if (m_bounds->getCenterX() > igs->m_nearBounds->getMaxX() - (container->getWidth()*0.5f)) {
		return m_animSideRTop;
	}
	return m_animCenterTop;
}

void Player::update(GameTimer* timer) {
	
    GameContainer* container = ARK2D::getContainer();
    m_movement->update(container, timer);
	
	//m_velocity->set(m_movement->getX() * 60.0f, m_movement->getY() * 60.0f);

	m_animCenterTop->update(timer);
	m_animCenter->update(timer);
	m_animSideTop->update(timer);
	m_animSide->update(timer);
	m_animSideRTop->update(timer);
	m_animSideR->update(timer);
	m_animGround->update(timer);
    
    if (m_hitTimer > 0.0f) {
        m_hitTimer += timer->getDelta();
        if (m_hitTimer >= m_hitDuration) {
            m_hitTimer = 0.0f;
        }
    }

    InGameState* igs = DefaultGame::getInstance()->stateInGame;
	if (igs->m_deathTimer > 0.0f) {
		if (!m_indeathloop) {
			signed int l = m_animDeath->getCurrentFrameIndex();	
			m_animDeath->update(timer);
			if (m_animDeath->getCurrentFrameIndex() < l) {
				m_indeathloop = true;
				m_animDeath->reset();
			}
		} 
	}
	
	m_animShield->update(timer);

	Input* in = ARK2D::getInput();


	// if (in->isKeyPressed(Input::KEY_L)) {
	// 	m_currentWeapon = (m_currentWeapon == WEAPON_GLOVE) 
	// 		? WEAPON_BOMB
	// 		: WEAPON_GLOVE;
	// }


	float angle = m_movement->m_lastAngle;
	bool keyDown = m_movement->m_stickPushing || m_movement->m_buttonPushing;

	if (m_movement->isLeftDown() && m_movement->isRightDown()) {

	} else if (m_movement->isLeftDown()) {
		//angle = 180;
		//keyDown = true;
		//m_facingLeft = true;
	} else if (m_movement->isRightDown()) {
		//angle = 0;
		//keyDown = true;
		//m_facingLeft = false;
	} 

	if (m_movement->isUpDown()) {
		//angle = -90;
		//keyDown = true;
		//m_facingUp = true;
	} 
	if (m_movement->isDownDown()) {
		//angle = 90;
		//keyDown = true;
		//m_facingUp = false;
	}
	if (m_movement->isJumpPressed() ) { 
		//if (m_grounded) {
		//	jump();
		//}
	}// else if (in->isKeyDown(Input::KEY_UP) && !m_grounded) {
	//	m_velocity->m_y -= 150 * timer->getDelta();
 	//}


	m_velocity->addX(additionalVelocity.getX() * timer->getDelta());
	m_velocity->addY(additionalVelocity.getY() * timer->getDelta()); 

	
	//m_velocity->setX(0);
	//m_velocity->setY(0);
	if (keyDown && canMove()) {
		m_moving = true;
		float distance = 1.0f;
		float maxAcceleration = 2000.0f;

		m_acceleration->set(0.0f, 0.0f);
		MathUtil::moveAngle<float>(m_acceleration, angle, maxAcceleration);

		m_velocity->addX(m_acceleration->getX() * timer->getDelta());
		m_velocity->addY(m_acceleration->getY() * timer->getDelta());
		
		
		m_velocity->restrictLength(m_velocityMax->length());
		
	} else {
		m_moving = false;
		m_acceleration->set(0.0f, 0.0f);
		
		m_velocity->frictionLength(500 * timer->getDelta());
		//m_velocity->frictionX(500 * timer->getDelta());
		//m_velocity->frictionY(500 * timer->getDelta());

	//	m_walkWobbleTimer = 0.0f;
	}

	gravity(1.0f);
	//move(1.0f);

	//ARK2D::getLog()->e(StringUtil::append("keydown ", keyDown));

	//string logStr = StringUtil::append("velocityx ", m_velocity->getX());
	//logStr += StringUtil::append("velocityx ", m_velocity->getY());
	//ARK2D::getLog()->e(logStr);



	m_bounds->setLocationByCenter(
	 	m_bounds->getCenterX() + (m_velocity->getX() * timer->getDelta()),
	 	m_bounds->getCenterY() + (m_velocity->getY() * timer->getDelta())
	);
}

bool Player::canMove() {
	return DefaultGame::getInstance()->stateInGame->m_deathTimer == 0.0f; 
}
