#pragma once
#include "Utils.h"

//defined in ArcanoidNew.cpp

//shared input variables
namespace input {
	extern int
		WINDOW_WIDTH,
		WINDOW_HEIGHT;

	extern bool
		arrowKeyRightPressed,
		arrowKeyLeftPressed,
		leftMouseButtonPressed;
	extern Vec2<int>
		mousePos;
}

//shared global start options
namespace options {
	extern bool
		ballResting;
	extern Vec2<int>
		playgroundStartPosition;
	extern int
		ballHealthDeafault,

		points,
		reward,

		playgroundWidth,
		playgroundHeight,

		spriteMouseWidth,
		spriteMouseHeight,

		spriteTrampolineWidth,
		spriteTrampolineHeight;

	extern float
		trampolineVelocity,
		ballVelocity;
}