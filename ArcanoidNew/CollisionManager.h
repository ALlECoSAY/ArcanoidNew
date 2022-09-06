#pragma once
#include "Utils.h"

//only 2 outcomes are handled: vertical and horizontal reflectiom(like in original arcanoid)
enum CollisionType
{
	HORIZONTAL,
	VERTICAL,
	NONE
};

//collided target
enum CollisionItem
{
	TRAMPOLIN,
	BORDER,
	BALL,
	TILE
};

class CollisionManager
{
public:
	template< typename _T, typename _M>
	static CollisionType areColliding(_T, _M);

	template< typename _T >
	static CollisionType isCollidingBorders(_T);

};

