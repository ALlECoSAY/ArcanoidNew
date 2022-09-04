#pragma once
#include "Framework.h"

class IShape
{
protected:
public:

	virtual void Move(float* velocityX, float* velocityY, unsigned int timeTicks) = 0;
	virtual void Update() = 0;
	
	//virtual void applyEffect(Effect* effect) = 0;
	virtual ~IShape() {
		
	};


	float _x, _y, *_velocity;
	Sprite* _sprite;
};



class RectangleShape :
	public IShape
{
public:
	// Inherited via IShape
	virtual void Move(float* velocityX, float* velocityY, unsigned int timeTicks) override;
	virtual void Update() override;

	RectangleShape(float x, float y, float* velocity, int width, int height, Sprite* sprite);
	virtual ~RectangleShape();

	int _width, _height;
};



class CircleShape :
	public IShape
{
public:
	// Inherited via IShape
	virtual void Move(float* velocityX, float* velocityY, unsigned int timeTicks) override;
	virtual void Update() override;

	CircleShape(float x, float y, float* velocity, int radius, Sprite* sprite);
	virtual ~CircleShape();

	int _radius;
};