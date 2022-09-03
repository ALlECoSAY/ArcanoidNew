#include "CollisionManager.h"
#include "IShape.h"
#include <math.h>

int
	CollisionManager::xPlayground = 0,
	CollisionManager::yPlayground = 0,
	CollisionManager::playgroundWidth = 0,
	CollisionManager::playgroundHeight = 0;



template<>
bool CollisionManager::areColliding(RectangleShape* r, RectangleShape* r2)
{
	float dx = fabs(r->_x - r2->_x);
	float dy = fabs(r->_y - r2->_y);
	if (dx < (r->_width + r2->_width) / 2 && dy < (r->_height + r2->_height) / 2) return true;

	return false;
};

template<>
bool CollisionManager::areColliding(CircleShape c1, CircleShape c2)
{
	return true;
};

template<>
bool CollisionManager::areColliding(CircleShape c, RectangleShape r2)
{

	return true;
};

template<>
bool CollisionManager::areColliding(RectangleShape r, CircleShape c)
{
	return CollisionManager::areColliding(r, c);
};











template<>
bool CollisionManager::isCollidingBorders(RectangleShape* r) {
	if (r->_x - r->_width/2 < xPlayground)						return true;
	if (r->_x + r->_width/2 > xPlayground + playgroundWidth)	return true;
	if (r->_y - r->_height/2 < yPlayground)						return true;
	if (r->_y + r->_height/2 > yPlayground + playgroundHeight)	return true;
	
	return false;
};


template<>
bool CollisionManager::isCollidingBorders(CircleShape* c) {
	if (c->_x - c->_radius / 2 < xPlayground)						return true;
	if (c->_x + c->_radius / 2 > xPlayground + playgroundWidth)		return true;
	if (c->_y - c->_radius / 2 < yPlayground)						return true;
	if (c->_y + c->_radius / 2 > yPlayground + playgroundHeight)	return true;

	return false;
	return true;
};

