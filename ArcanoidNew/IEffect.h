#pragma once
#include <list>;
#include "IShape.h";

class IEffect
{
public:
	virtual void Apply() = 0;
	virtual void Purge() = 0;
	//IShape targetShape;
	IShape* _targetShape;
};

class ITemporalEffect : public IEffect
{
public:
	virtual bool Update(unsigned int deltaTime) = 0;
	int _duration;
};

class AreaVelocityEffect : public IEffect
{
public:
	AreaVelocityEffect(std::list<Vec2<float>> dots, IShape* targetShape);
	void Apply();
	void Purge();
private:
	std::list<Vec2<float>> _dots;
};

class SizeChangeEffect : public ITemporalEffect
{
public:
	SizeChangeEffect(float miltiplier, int duration, IShape* targetShape);
	void Apply();
	void Purge();
	bool Update(unsigned int deltaTime);
private:
	float _multiplier;
};

