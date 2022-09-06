#pragma once
#include <list>;
#include "IShape.h";

class IEffect
{
public:
	virtual void Apply() = 0; //applies new modification value
	virtual void Purge() = 0; //sets default modificator value
	IShape* _targetShape; //afflicted shape
};

class ITemporalEffect : public IEffect
{
public:
	virtual bool Update(unsigned int deltaTime) = 0;//duration-=deltaTime>0? --Purge condition's check
	int _duration;
};

class AreaVelocityEffect : public IEffect
{
public:
	AreaVelocityEffect(std::list<Vec2<float>> dots, IShape* targetShape); //applies as written in README
	void Apply();
	void Purge();
private:
	std::list<Vec2<float>> _dots;
};

class SizeChangeEffect : public ITemporalEffect
{
public:
	SizeChangeEffect(float miltiplier, int duration, IShape* targetShape); //changes size modificator
	void Apply();
	void Purge();
	bool Update(unsigned int deltaTime);
private:
	float _multiplier;
};

