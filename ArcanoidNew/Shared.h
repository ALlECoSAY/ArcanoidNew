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
	extern Vec2<int>
		playgroundStartPosition;
	extern int
		playgroundWidth,
		playgroundHeight,

		spriteMouseWidth,
		spriteMouseHeight,

		spriteTrampolineWidth,
		spriteTrampolineHeight;
}