#pragma once
#include "Utils.h"

enum CollisionType
{
	HORIZONTAL,
	VERTICAL,
	NONE
};
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

