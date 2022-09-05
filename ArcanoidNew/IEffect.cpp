#include "IEffect.h"
#include <math.h>

AreaVelocityEffect::AreaVelocityEffect(std::list<Vec2<float>> dots, IShape* targetShape) {
	_targetShape = targetShape;
	_dots = dots;
};

void AreaVelocityEffect::Apply() {
	int c = 0;
	for (Vec2<float> dot : _dots)
	{
		if (_targetShape->_position.x < dot.x) c++;
		if (_targetShape->_position.y < dot.y) c++;
	}
	if (pow(-1, c) < 0) {
		_targetShape->_velocityModifier = 0.5;
	}
	else {
		_targetShape->_velocityModifier = 1.5;
	}
};

void AreaVelocityEffect::Purge() {
	int c = 0;
	for (Vec2<float> dot : _dots)
	{
		if (_targetShape->_position.x < dot.x) c++;
		if (_targetShape->_position.y < dot.y) c++;
	}
	if (pow(-1, c) < 0) {

	}
	else {
		_targetShape->_velocityModifier = 1;
	}
}




SizeChangeEffect::SizeChangeEffect(float multiplier, int duration, IShape* targetShape) {
	_multiplier = multiplier;
	_duration = duration;
	_targetShape = targetShape;
};

void SizeChangeEffect::Apply() {
	_targetShape->_sizeModifier *= _multiplier;
	_targetShape->OnEffectChange();
};

void SizeChangeEffect::Purge() {
	_targetShape->_sizeModifier /= _multiplier;
	_targetShape->OnEffectChange();
}

bool SizeChangeEffect::Update(unsigned int deltaTime) { 
	_duration -= deltaTime;
	return _duration > 0 ? true : false;
};