#include "CollisionManager.h"
#include "IShape.h"
#include <math.h>
#include "Shared.h"

using namespace options;

template<>
CollisionType CollisionManager::areColliding(RectangleShape* r1, RectangleShape* r2)
{
	Vec2<float> delta = r1->_position - r2->_position;
	float dx = fabs(delta.x);
	float dy = fabs(delta.y);
	if (!(dx < (r1->_width + r2->_width) / 2 && dy < (r1->_height + r2->_height) / 2)) return NONE;
	dx = (r1->_width + r2->_width) / 2 - dx;
	dy = (r1->_height + r2->_height) / 2 - dy;
	if (dy < dx) return VERTICAL;
	return HORIZONTAL;
};

template<>
CollisionType CollisionManager::areColliding(CircleShape* c, RectangleShape* r)
{
	Vec2<float> delta = c->_position - r->_position;
	float dx = fabs(delta.x);
	float dy = fabs(delta.y);

	if (dx >= (r->_width / 2 + c->_radius)) { return NONE; }
	if (dy >= (r->_height / 2 + c->_radius)) { return NONE; }

	//v kakuyu storonu deflect tablica istinnosti v tg #tbistdeflect
	Vec2<float> v = c->_velocityDirecrion;
	if (v.x <= 0 && v.y > 0 && delta.x <= 0) 		return VERTICAL;
	if (v.x > 0 && v.y > 0 && delta.x > 0) 			return VERTICAL;
	if (v.x <= 0 && v.y <= 0 && delta.x <= 0)		return VERTICAL;
	if (v.x > 0 && v.y <= 0 && delta.x > 0)			return VERTICAL;
	if (v.x <= 0 && v.y > 0 && delta.y > 0)			return HORIZONTAL;
	if (v.x > 0 && v.y > 0 && delta.y > 0)			return HORIZONTAL;
	if (v.x <= 0 && v.y <= 0 && delta.y <= 0)		return HORIZONTAL;
	if (v.x > 0 && v.y <= 0 && delta.y <= 0)		return HORIZONTAL;

	dx = c->_radius + r->_width / 2 - dx;
	dy = c->_radius + r->_height / 2 - dy;
	if (dy < dx) return VERTICAL;
	return HORIZONTAL;
};

template<>
CollisionType CollisionManager::areColliding(RectangleShape* r, CircleShape* c)
{
	return CollisionManager::areColliding(c,r);
};



template<>
CollisionType CollisionManager::isCollidingBorders(RectangleShape* r) {
	if (r->_position.x - r->_width/2 < playgroundStartPosition.x)						return HORIZONTAL;
	if (r->_position.x + r->_width/2 > playgroundStartPosition.x + playgroundWidth)		return HORIZONTAL;
	if (r->_position.y - r->_height/2 < playgroundStartPosition.y)						return VERTICAL;
	if (r->_position.y + r->_height/2 > playgroundStartPosition.y + playgroundHeight)	return VERTICAL;
	
	return NONE;
};

template<>
CollisionType CollisionManager::isCollidingBorders(CircleShape* c) {
	if (c->_position.x - c->_radius < playgroundStartPosition.x)						return HORIZONTAL;
	if (c->_position.x + c->_radius > playgroundStartPosition.x + playgroundWidth)		return HORIZONTAL;
	if (c->_position.y - c->_radius < playgroundStartPosition.y)						return VERTICAL;
	if (c->_position.y + c->_radius > playgroundStartPosition.y + playgroundHeight)		return VERTICAL;

	return NONE;
};

