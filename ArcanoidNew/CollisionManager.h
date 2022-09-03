#pragma once
enum CollisionType
{
	HORIZONTAL,
	VERTICAL,
	NONE
};

class CollisionManager
{
public:
	template< typename _T, typename _M>
	static CollisionType areColliding(_T, _M);

	template< typename _T >
	static CollisionType isCollidingBorders(_T);

	static int
		xPlayground,
		yPlayground,
		playgroundWidth,
		playgroundHeight;
};

