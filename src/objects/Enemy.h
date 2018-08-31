
#ifndef ARK2D_DEFAULTGAME_ENEMY_H_
#define ARK2D_DEFAULTGAME_ENEMY_H_

#include "../objects/ScrollObject.h"
#include <ARK2D/Core/Graphics/Animation.h>


class Enemy : public ScrollObject {
	public:
		static const unsigned int TYPE_CROW = 0;
	public:
		static AnimationFrameList* s_animCrow;
		static AnimationFrameList* s_animCrowAttack;
		
		Animation* m_animation;
		unsigned int m_type;
		//float m_stopAt;

		float m_fireTimer;
		float m_fireDuration;
		bool fireAnim;
		bool fireAnimFired;

		Enemy();
		void die();
		void reset();
		void start(int type);
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
};

#endif