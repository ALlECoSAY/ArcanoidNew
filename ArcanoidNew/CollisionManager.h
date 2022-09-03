#pragma once

class CollisionManager
{
public:
	template< typename _T, typename _M>
	static bool areColliding(_T, _M);

	template< typename _T >
	static bool isCollidingBorders(_T);

	static int
		xPlayground,
		yPlayground,
		playgroundWidth,
		playgroundHeight;
};

enum CollisionType
{
	HORIZONTAL,
	VERTICAL,
	NONE
};