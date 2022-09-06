#include "IShape.h"
#include <iostream>
#include "Framework.h"
#include "CollisionManager.h"
#include "Shared.h"


using namespace input;
using namespace options;

//Rectangle
//-----------------------------------------------------------------------------------------------------------------------------------------------------
RectangleShape::RectangleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite) {
	_position = position;
	_velocity = velocity;
	_velocityDirecrion = velocityDirecrion;
	_defaultWidth = width;
	_width = width;
	_height = height;
	_sprite = sprite;
	setSpriteSize(sprite, width, height);
};
RectangleShape::~RectangleShape() {

};
void RectangleShape::Move(unsigned int timeTicks) {
	
	Vec2<float> deltaPos = _velocityDirecrion * timeTicks * _velocity;

	_position = _position + deltaPos;

	//check on borders collision
	CollisionType t = CollisionManager::isCollidingBorders(this);
	if (t != NONE) {
		float adhereBorderCoordinate;
		
		switch (t)
		{
			case HORIZONTAL:
				adhereBorderCoordinate = _position.x - _width / 2 < playgroundStartPosition.x ? playgroundStartPosition.x + _width / 2 :
					_position.x + _width / 2 > playgroundStartPosition.x + playgroundWidth ? playgroundStartPosition.x + playgroundWidth - _width / 2 :
					_position.x;
				break;
			case VERTICAL:
				adhereBorderCoordinate = _position.y - _height / 2 < playgroundStartPosition.y ? playgroundStartPosition.y + _height / 2 :
					_position.y + _height / 2 > playgroundStartPosition.y + playgroundHeight ? playgroundStartPosition.y + playgroundHeight - _height / 2 :
					_position.y;
				break;
			case NONE:
			default:
				break;
		}
		this->OnCollide(t, BORDER, adhereBorderCoordinate);
	}
};
bool RectangleShape::Update(unsigned int timeTicks) {
	if (_velocity != 0) if (_velocityDirecrion.x != 0 || _velocityDirecrion.y != 0) {
		Move(timeTicks);
	}
	drawSprite(_sprite, _position.x - _width / 2, _position.y - _height / 2);
	return true;
};
void RectangleShape::OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) {

}
void RectangleShape::OnEffectChange() {
	_width = _defaultWidth * (_sizeModifier > 2 ? 2 : _sizeModifier < 0.4 ? 0.4 : _sizeModifier);
	setSpriteSize(_sprite, _width , _height);

}
void RectangleShape::SetDirection(Vec2<float> direction) {
	_velocityDirecrion = direction;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//Circle
//-----------------------------------------------------------------------------------------------------------------------------------------------------
CircleShape::CircleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int radius, Sprite* sprite) {
	_position = position;
	_velocity = velocity;
	_velocityDirecrion = velocityDirecrion;
	_radius = radius;
	_sprite = sprite;
	setSpriteSize(sprite, radius*2, radius*2);
};
CircleShape::~CircleShape() {

};
void CircleShape::Move(unsigned int timeTicks) {

	Vec2<float> deltaPos = _velocityDirecrion * timeTicks * _velocity * _velocityModifier;

	_position = _position + deltaPos;

	//check on borders collision
	CollisionType t = CollisionManager::isCollidingBorders(this);
	if (t != NONE) {
		float adhereBorderCoordinate;
		switch (t)
		{
		case HORIZONTAL:
			adhereBorderCoordinate = _position.x - _radius < playgroundStartPosition.x ? playgroundStartPosition.x + _radius :
				_position.x + _radius > playgroundStartPosition.x + playgroundWidth ? playgroundStartPosition.x + playgroundWidth - _radius :
				_position.x;
			break;
		case VERTICAL:
			adhereBorderCoordinate = _position.y - _radius < playgroundStartPosition.y ? playgroundStartPosition.y + _radius :
				_position.y + _radius > playgroundStartPosition.y + playgroundHeight ? playgroundStartPosition.y + playgroundHeight - _radius :
				_position.y;
			break;
		case NONE:
		default:
			break;
		}
		this->OnCollide(t, BORDER, adhereBorderCoordinate);
	}
};
bool CircleShape::Update(unsigned int timeTicks) {
	if (_velocity != 0) if (_velocityDirecrion.x != 0 || _velocityDirecrion.y != 0) {
		Move(timeTicks);
		//TODO
		
	}
	drawSprite(_sprite, _position.x - _radius, _position.y - _radius);
	return true;
};
void CircleShape::OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) {

}
void CircleShape::OnEffectChange() {

}
void CircleShape::SetDirection(Vec2<float> direction) {
	_velocityDirecrion = direction;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------






//Trampoline
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Trampoline::Trampoline(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite) 
	: RectangleShape::RectangleShape(position, velocity, velocityDirecrion, width, height, sprite) {}
void Trampoline::OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) {
	switch (item)
	{
	case TRAMPOLIN:
		break;
	case BORDER:
		switch (type)
		{
		case HORIZONTAL:
			_position.x = adhereBorderCoordinate;
			_velocityDirecrion.x = -_velocityDirecrion.x;
			break;
		case VERTICAL:
			_position.y = adhereBorderCoordinate;
			_velocityDirecrion.y = -_velocityDirecrion.y;
			break;
		default:

			break;
		}
		break;
	case BALL:
		break;
	case TILE:
		break;
	default:
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//Ball old rectangular, deprecated
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Ball::Ball(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite)
	: RectangleShape::RectangleShape(position, velocity, velocityDirecrion, width, height, sprite) {}
void Ball::OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) {
	switch (item)
	{
	case TRAMPOLIN:
		switch (type) {
		case HORIZONTAL:
			_position.x = adhereBorderCoordinate;
			_velocityDirecrion.x = - _velocityDirecrion.x;
			break;
		case VERTICAL:
			_position.y = adhereBorderCoordinate;
			_velocityDirecrion.y = - _velocityDirecrion.y;
			if (leftMouseButtonPressed) {
				float
					dx = mousePos.x - _position.x,
					dy = mousePos.y - _position.y,
					c = sqrtf(powf(dx, 2) + powf(dy, 2));

				_velocityDirecrion.x = _velocity * dx / c;
				_velocityDirecrion.y = _velocity * dy / c;
			}
			break;
		default:
			break;
		}
		break;
	case BORDER:
		switch (type)
		{
			case HORIZONTAL:
				_position.x = adhereBorderCoordinate;
				_velocityDirecrion.x = -_velocityDirecrion.x;
				break;
			case VERTICAL:
				_position.y = adhereBorderCoordinate;
				_velocityDirecrion.y = -_velocityDirecrion.y;
				break;
			default:

				break;
		}
		break;
	case BALL:
		break;
	case TILE:
		switch (type)
		{
		case HORIZONTAL:
			_position.x = adhereBorderCoordinate;
			_velocityDirecrion.x = -_velocityDirecrion.x;
			break;
		case VERTICAL:
			_position.y = adhereBorderCoordinate;
			_velocityDirecrion.y = -_velocityDirecrion.y;
			break;
		default:

			break;
		}
		break;
	default:
		break;
	}

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------

//Tile
//-----------------------------------------------------------------------------------------------------------------------------------------------------
Tile::Tile(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite)
	: RectangleShape::RectangleShape(position, velocity, velocityDirecrion, width, height, sprite) {}
void Tile::OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) {
	_health -= 1;
}
bool Tile::Update(unsigned int timeTicks) {
	if (_health <= 0) return false;
	RectangleShape::Update(timeTicks);
	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//Ball
//-----------------------------------------------------------------------------------------------------------------------------------------------------
BallCircle::BallCircle(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int radius, Sprite* sprite)
	: CircleShape::CircleShape(position, velocity, velocityDirecrion, radius, sprite) { 
	_health = ballHealthDeafault;
}
void BallCircle::OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) {

	switch (item)
	{
	case TRAMPOLIN:
		switch (type) {
		case HORIZONTAL:
			_position.x = adhereBorderCoordinate;
			_velocityDirecrion.x = -_velocityDirecrion.x;
			break;
		case VERTICAL:
			_position.y = adhereBorderCoordinate;
			_velocityDirecrion.y = -_velocityDirecrion.y;
			if (leftMouseButtonPressed) {
				float
					dx = mousePos.x - _position.x,
					dy = mousePos.y - _position.y,
					c = sqrtf(powf(dx, 2) + powf(dy, 2));

				_velocityDirecrion.x = _velocity * dx / c;
				_velocityDirecrion.y = _velocity * dy / c;
			}
			break;
		default:
			break;
		}
		break;
	case BORDER:
		switch (type)
		{
		case HORIZONTAL:
			_position.x = adhereBorderCoordinate;
			_velocityDirecrion.x = -_velocityDirecrion.x;
			break;
		case VERTICAL:
			if (_position.y > _radius) { _health--; ballHealthDeafault--; ballResting = true; }
			_position.y = adhereBorderCoordinate;
			_velocityDirecrion.y = -_velocityDirecrion.y;
			break;
		default:

			break;
		}
		break;
	case BALL:
		break;
	case TILE:
		switch (type)
		{
		case HORIZONTAL:
			_position.x = adhereBorderCoordinate;
			_velocityDirecrion.x = -_velocityDirecrion.x;
			break;
		case VERTICAL:
			_position.y = adhereBorderCoordinate;
			_velocityDirecrion.y = -_velocityDirecrion.y;
			break;
		default:

			break;
		}
		break;
	default:
		break;
	}

}
bool BallCircle::Update(unsigned int timeTicks) {
	if(_health < 1) return false;
	CircleShape::Update(timeTicks);
	return true;
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------





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
