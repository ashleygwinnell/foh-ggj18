#ifndef ARK2D_DEFAULTGAME_SCROLLOBJECT_H_
#define ARK2D_DEFAULTGAME_SCROLLOBJECT_H_

#include "Object.h"

class ScrollObject : public GameObject {
	public:
		ARK::Core::Geometry::Shape<float>* m_bounds;
		float percentageX;
		float percentageY;

		float startx;
		float starty;
		float endx; 
		float endy;
		float actualx;
		float actualy;
		float sizex;
		float sizey;
		float z;
		float speed;

		float m_stopAt;

		float shadowy;

		ScrollObject();
		virtual void recalcPositions();
		virtual void update(GameContainer* container, GameTimer* timer);
		virtual void render(GameContainer* container, Renderer* r);
		virtual void renderImage(GameContainer* container, Renderer* r, Image* img, float offsety=0.0f, float offsetx=0.0f);
		virtual void renderBounds(GameContainer* container, Renderer* r);
		virtual ~ScrollObject();
};

#endif
