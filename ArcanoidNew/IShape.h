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
	virtual bool Update(unsigned int timeTicks) = 0;
	virtual void OnEffectChange() = 0;
	
	//virtual void applyEffect(Effect* effect) = 0;
	virtual ~IShape() {
		
	};


	float _velocity;
	Vec2<float> _position,
		_velocityDirecrion;
	float _sizeModifier = 1, _velocityModifier = 1;
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
	virtual bool Update(unsigned int timeTicks) override;
	void OnEffectChange();

	RectangleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	virtual ~RectangleShape();

	int _width, _height;
private:
	int _defaultWidth;
};

class CircleShape :
	public IShape
{
protected:
	virtual void Move(unsigned int timeTicks) override;
public:
	// Inherited via IShape
	virtual void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate) override;
	virtual void SetDirection(Vec2<float> direction) override;
	virtual bool Update(unsigned int timeTicks) override;
	void OnEffectChange();

	CircleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int radius, Sprite* sprite);
	virtual ~CircleShape();

	int _radius;
};

class BallCircle : public CircleShape
{
public:
	BallCircle(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int radius, Sprite* sprite);
	void OnCollide(CollisionType type, CollisionItem item, float adhereBorderCoordinate);
	bool Update(unsigned int timeTicks);
private:
	int _health = 3;
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
	bool Update(unsigned int timeTicks);

private:
	int _health = 1;
};




