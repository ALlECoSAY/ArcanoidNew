#pragma once
#include "Framework.h"
#include "Utils.h"

class IShape
{
protected:
	virtual void Move(unsigned int timeTicks) = 0;
	virtual void OnCollide() = 0;
public:

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
	virtual void OnCollide() override;
public:
	// Inherited via IShape
	virtual void SetDirection(Vec2<float> direction) override;
	virtual void Update(unsigned int timeTicks) override;

	RectangleShape(Vec2<float> position, float velocity, Vec2<float> velocityDirecrion, int width, int height, Sprite* sprite);
	virtual ~RectangleShape();

	int _width, _height;
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