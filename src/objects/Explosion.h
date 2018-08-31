#ifndef ARK2D_DEFAULTGAME_EXPLOSION_H_
#define ARK2D_DEFAULTGAME_EXPLOSION_H_

#include "../objects/ScrollObject.h"
#include <ARK2D/Core/Graphics/Animation.h>

class Explosion : public ScrollObject {
	public:
		static AnimationFrameList* s_explosion1;
		static AnimationFrameList* s_explosion2;

		unsigned int m_type;
		Animation* m_animation;
		Explosion();
		virtual void reset();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
};

#endif
