#pragma once
#include "Framework.h"
#include "CollisionManager.h"
#include "Utils.h"

class IShape
{
protected:
	virtual void Move(unsigned int timeTicks) = 0;
public:

	virtual void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) = 0;
	virtual void SetDirection(Vec2<float> direction) = 0;
	virtual void Update(unsigned int timeTicks) = 0;
	
	//virtual void applyEffect(Effect* effect) = 0;
	virtual ~IShape() {
		
	};


	float _velocity;
	Vec2<float> _position,
		_velocityDirecrion;
	Sprite* _sprite;
};



class RectangleShape :
	public IShape
{
protected:
	virtual void Move(unsigned int timeTicks) override;
public:
	// Inherited via IShape
	virtual void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) override;
	virtual void SetDirection(Vec2<float> direction) override;
	virtual void Update(unsigned int timeTicks) override;

	RectangleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	virtual ~RectangleShape();

	int _width, _height;
};

class Trampoline : public RectangleShape
{
public:
	Trampoline(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate);

private:

};

class Ball : public RectangleShape
{
public:
	Ball(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate);

private:

};

class Tile : public RectangleShape
{
public:
	Tile(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate);

private:
	int _health = 1;
};





//class CircleShape :
//	public IShape
//{
//public:
//	// Inherited via IShape
//	virtual void Move(float* velocityX, float* velocityY, unsigned int timeTicks) override;
//	virtual void Update(unsigned int timeTicks) override;
//
//	CircleShape(float x, float y, float* velocity, int radius, Sprite* sprite);
//	virtual ~CircleShape();
//
//	int _radius;
//};