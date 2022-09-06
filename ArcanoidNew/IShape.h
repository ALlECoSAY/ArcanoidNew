#pragma once
#include "Framework.h"
#include "CollisionManager.h"
#include "Utils.h"

class IShape
{
protected:
	virtual void Move(unsigned int timeTicks) = 0; //moves position adding delta vector = velocity * timeTics * basis
	
public:
	virtual void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) = 0; //adhere border coordinate aligns shape to the conflicted coord without intersection
	virtual void SetDirection(Vec2<float> direction) = 0; //sets velocity basis
	virtual bool Update(unsigned int timeTicks) = 0; //updates every tick
	virtual void OnEffectChange() = 0; //updates size/other affected variables
	
	virtual ~IShape() {
		
	};


	float _velocity;
	Vec2<float> 
		_position,
		_velocityDirecrion;
	float _sizeModifier = 1, 
		_velocityModifier = 1;
	Sprite* _sprite;
};


//different to circle collision scheme && size variables
class RectangleShape :
	public IShape
{
protected:
	virtual void Move(unsigned int timeTicks) override;	
public:
	virtual void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) override;
	virtual void SetDirection(Vec2<float> direction) override;
	virtual bool Update(unsigned int timeTicks) override;
	void OnEffectChange();

	RectangleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	virtual ~RectangleShape();

	int _width, 
		_height;
private:
	int _defaultWidth;
};


class CircleShape :
	public IShape
{
protected:
	virtual void Move(unsigned int timeTicks) override;
public:
	virtual void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) override;
	virtual void SetDirection(Vec2<float> direction) override;
	virtual bool Update(unsigned int timeTicks) override;
	void OnEffectChange();

	CircleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int radius, Sprite* sprite);
	virtual ~CircleShape();

	int _radius;
};

//acting ball
class BallCircle : public CircleShape
{
public:
	BallCircle(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int radius, Sprite* sprite);
	void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate);
	bool Update(unsigned int timeTicks);
private:
	int _health = 3;
};

// a.k.a 'Platform'
class Trampoline : public RectangleShape
{
public:
	Trampoline(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate);

private:

};

//deprecated
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
	bool Update(unsigned int timeTicks); //returns false when health<1

private:
	int _health = 1;
};




