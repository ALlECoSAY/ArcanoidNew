#pragma once
#include "Utils.h"

namespace input {
	extern bool
		arrowKeyRightPressed,
		arrowKeyLeftPressed,
		leftMouseButtonPressed;
	extern Vec2<int>
		mousePos;
}

namespace options {
	extern bool
		ballResting;
	extern Vec2<int>
		playgroundStartPosition;
	extern int
		ballHealthDeafault,

		points,

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