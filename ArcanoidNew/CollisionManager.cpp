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

//template<>
//CollisionType CollisionManager::areColliding(CircleShape* c1, CircleShape* c2)
//{
//	return NONE;
//};
//
//template<>
//CollisionType CollisionManager::areColliding(CircleShape* c, RectangleShape* r2)
//{
//
//	return NONE;
//};
//
//template<>
//CollisionType CollisionManager::areColliding(RectangleShape* r, CircleShape* c)
//{
//	return CollisionManager::areColliding(r, c);
//};
//
//









template<>
CollisionType CollisionManager::isCollidingBorders(RectangleShape* r) {
	if (r->_position.x - r->_width/2 < playgroundStartPosition.x)						return HORIZONTAL;
	if (r->_position.x + r->_width/2 > playgroundStartPosition.x + playgroundWidth)		return HORIZONTAL;
	if (r->_position.y - r->_height/2 < playgroundStartPosition.y)						return VERTICAL;
	if (r->_position.y + r->_height/2 > playgroundStartPosition.y + playgroundHeight)	return VERTICAL;
	
	return NONE;
};


//template<>
//CollisionType CollisionManager::isCollidingBorders(CircleShape* c) {
//	if (c->_x - c->_radius / 2 < xPlayground)						return HORIZONTAL;
//	if (c->_x + c->_radius / 2 > xPlayground + playgroundWidth)		return HORIZONTAL;
//	if (c->_y - c->_radius / 2 < yPlayground)						return VERTICAL;
//	if (c->_y + c->_radius / 2 > yPlayground + playgroundHeight)	return VERTICAL;
//
//	return NONE;
//};
//
