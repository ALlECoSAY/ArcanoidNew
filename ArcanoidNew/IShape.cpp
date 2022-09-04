#include "IShape.h"
#include "Framework.h"
#include "CollisionManager.h"
#include "Shared.h"
using namespace input;
using namespace options;


RectangleShape::RectangleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite) {
	_position = position;
	_velocity = velocity;
	_velocityDirecrion = velocityDirecrion;
	_width = width;
	_height = height;
	_sprite = sprite;
	setSpriteSize(sprite, width, height);
};

void RectangleShape::Move(unsigned int timeTicks) {
	
	Vec2<float> deltaPos = _velocityDirecrion * timeTicks * _velocity;

	_position = _position + deltaPos;

	//check on borders collision
	CollisionType t = CollisionManager::isCollidingBorders(this);
	
	switch (t)
	{
		case HORIZONTAL:
			_velocityDirecrion.x = -_velocityDirecrion.x;
			break;
		case VERTICAL:
			_velocityDirecrion.y = -_velocityDirecrion.y;
			break;
		case NONE:
		default:
			break;
	}
	if (t!=NONE) {
		_position.x = _position.x - _width / 2 < playgroundStartPosition.x ? playgroundStartPosition.x + _width / 2 :
			_position.x + _width / 2 > playgroundStartPosition.x + playgroundWidth ? playgroundStartPosition.x + playgroundWidth - _width / 2 :
			_position.x;
		_position.y = _position.y - _height / 2 < playgroundStartPosition.y ? playgroundStartPosition.y + _height / 2 :
			_position.y + _height / 2 > playgroundStartPosition.y + playgroundHeight ? playgroundStartPosition.y + playgroundHeight - _height / 2 :
			_position.y;
	}
};

void RectangleShape::Update(unsigned int timeTicks) {
	if(_velocity!=0) if(_velocityDirecrion.x!=0 || _velocityDirecrion.y != 0) Move(timeTicks);
	drawSprite(_sprite, _position.x - _width / 2, _position.y - _height / 2);
};

void RectangleShape::OnCollide() {}

void RectangleShape::SetDirection(Vec2<float> direction) {
	_velocityDirecrion = direction;
}


RectangleShape::~RectangleShape() {

};





//CircleShape::CircleShape(float x, float y, float* velocity, int radius, Sprite* sprite)
//{
//	_position.x = x;
//	_position.y = y;
//	_radius = radius;
//	_sprite = sprite;
//	_velocity = velocity;
//};
//
//void CircleShape::Move(float* velocityX, float* velocityY, unsigned int timeTicks)
//{
//	float
//		v = *_velocity,
//		vX = *velocityX,
//		vY = *velocityY;
//	_position.x += v * vX * timeTicks;
//	_y += v * vY * timeTicks;
//	if (CollisionManager::isCollidingBorders(this)) {
//		/*_position.x = _position.x - _radius / 2 < CollisionManager::xPlayground ? CollisionManager::xPlayground + _radius / 2 :
//			_position.x + _radius / 2 > CollisionManager::xPlayground + CollisionManager::playgroundWidth ? CollisionManager::xPlayground + CollisionManager::playgroundWidth - _radius / 2 :
//			_position.x;
//		_y = _y - _radius / 2 < CollisionManager::yPlayground ? CollisionManager::yPlayground + _radius / 2 :
//			_y + _radius / 2 > CollisionManager::yPlayground + CollisionManager::playgroundHeight ? CollisionManager::yPlayground + CollisionManager::playgroundHeight - _radius / 2 :
//			_y;*/
//	}
//
//};
//
//void CircleShape::Update(unsigned int timeTicks)
//{
//};
//
//CircleShape::~CircleShape()
//{
//};
