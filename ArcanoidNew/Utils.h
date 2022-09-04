#pragma once

template<typename T>
class Vec2
{

public:
	T x, y;

	Vec2() : x(T(0)), y(T(0)) {}
	Vec2(const T& xx) : x(xx), y(xx) {}
	Vec2(T xx, T yy) : x(xx), y(yy) {}
	~Vec2();

	T length() {
		return sqrt(x * x + y * y);
	}
	
	Vec2<T> operator + (const Vec2<T>& v) const
	{
		return Vec2<T>(x + v.x, y + v.y, z + v.z);
	}
	Vec2<T> operator - (const Vec2<T>& v) const
	{
		return Vec2<T>(x - v.x, y - v.y, z - v.z);
	}
	Vec2<T> operator * (const T& r) const
	{
		return Vec2<T>(x * r, y * r, z * r);
	}
	
};
