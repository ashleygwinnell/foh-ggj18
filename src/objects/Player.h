#ifndef ARK2D_DEFAULTGAME_PLAYER_H_
#define ARK2D_DEFAULTGAME_PLAYER_H_

#include "../objects/EightWayMovementComponent.h"
#include "../objects/Object.h"
class InGameState;

class Player : public Object {
	public:	
		float speed;
		float z;
		bool m_moving;
		EightWayMovementComponent* m_movement;
		Vector2<float> additionalVelocity;

		Animation* m_animCenterTop;
		Animation* m_animCenter;
		Animation* m_animSideTop;
		Animation* m_animSide;
		Animation* m_animSideRTop;
		Animation* m_animSideR;
		Animation* m_animGround;
		Animation* m_animDeath;
		Animation* m_animDeathLoop;
		Image* m_shadow;

		bool m_shield;
		Animation* m_animShield;

		bool m_indeathloop;

		float m_hitTimer;
		float m_hitDuration;

		float m_fireTimer;
		float m_fireDuration;

		float getPercentageX();
		float getPercentageY();

		Player();
		void reset();
		void hit();
		void die();
		bool canBeHit();
		void update(GameTimer* timer);
		Animation* getCurrentAnimation();
		bool canMove();
}; 


#endif
