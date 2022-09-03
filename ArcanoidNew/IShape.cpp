#include "IShape.h"
#include "Framework.h"
#include "CollisionManager.h"


RectangleShape::RectangleShape(float x, float y, float* velocity, int width, int height, Sprite* sprite) {
	_x = x;
	_y = y;
	_velocity = velocity;
	_width = width;
	_height = height;
	_sprite = sprite;
	setSpriteSize(sprite, width, height);
};

void RectangleShape::Move(float* velocityX, float* velocityY, unsigned int timeTicks) {
	float 
		v = *_velocity,
		vX = *velocityX,
		vY = *velocityY;


	_x += v * vX * timeTicks;
	_y += v * vY * timeTicks;
	if (CollisionManager::isCollidingBorders(this)) {
		_x = _x - _width/2 < CollisionManager::xPlayground ? CollisionManager::xPlayground + _width/2 :
			_x + _width/2 > CollisionManager::xPlayground + CollisionManager::playgroundWidth ? CollisionManager::xPlayground + CollisionManager::playgroundWidth - _width/2 :
				_x;
		_y = _y - _height/2 < CollisionManager::yPlayground ? CollisionManager::yPlayground + _height/2 :
			_y + _height/2 > CollisionManager::yPlayground + CollisionManager::playgroundHeight ? CollisionManager::yPlayground + CollisionManager::playgroundHeight - _height/2 :
			_y;
	}
};

void RectangleShape::Update() {
	drawSprite(_sprite, _x - _width / 2, _y - _height / 2);
};

RectangleShape::~RectangleShape() {

}





CircleShape::CircleShape(float x, float y, float* velocity, int radius, Sprite* sprite)
{
	_x = x;
	_y = y;
	_radius = radius;
	_sprite = sprite;
	_velocity = velocity;
};

void CircleShape::Move(float* velocityX, float* velocityY, unsigned int timeTicks)
{
	float
		v = *_velocity,
		vX = *velocityX,
		vY = *velocityY;
	_x += v * vX * timeTicks;
	_y += v * vY * timeTicks;
	if (CollisionManager::isCollidingBorders(this)) {
		/*_x = _x - _radius / 2 < CollisionManager::xPlayground ? CollisionManager::xPlayground + _radius / 2 :
			_x + _radius / 2 > CollisionManager::xPlayground + CollisionManager::playgroundWidth ? CollisionManager::xPlayground + CollisionManager::playgroundWidth - _radius / 2 :
			_x;
		_y = _y - _radius / 2 < CollisionManager::yPlayground ? CollisionManager::yPlayground + _radius / 2 :
			_y + _radius / 2 > CollisionManager::yPlayground + CollisionManager::playgroundHeight ? CollisionManager::yPlayground + CollisionManager::playgroundHeight - _radius / 2 :
			_y;*/
	}

}

void CircleShape::Update()
{
}

CircleShape::~CircleShape()
{
}
