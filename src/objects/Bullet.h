
#ifndef ARK2D_DEFAULTGAME_BULLET_H_
#define ARK2D_DEFAULTGAME_BULLET_H_

#include "../objects/ScrollObject.h"
#include <ARK2D/Core/Graphics/Animation.h>


class Bullet : public ScrollObject {
	public:
		static AnimationFrameList* s_animCenterTop;
		static AnimationFrameList* s_animCenter;
		static AnimationFrameList* s_animCenterBottom;
		
		static AnimationFrameList* s_animSideTop;
		static AnimationFrameList* s_animSide;
		static AnimationFrameList* s_animSideBottom;

		static AnimationFrameList* s_animSideRTop;
		static AnimationFrameList* s_animSideR;
		static AnimationFrameList* s_animSideRBottom;
		
		static AnimationFrameList* s_tornado;

		Animation* m_animation;

		bool player;
		Bullet();
		virtual void recalcPositions();
		virtual void reset();
		AnimationFrameList* getAnimationForPosition();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
};

#endif