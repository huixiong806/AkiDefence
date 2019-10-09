#pragma once
#include<cmath>
template<class T>
class Vec2
{
public:
	T x, y;
	Vec2() { x = T(), y = T(); }
	Vec2(const T& xx, const T& yy) :x(xx), y(yy) {}
	Vec2(const Vec2& tar) { x = tar.x; y = tar.y; }
	bool operator ==(const Vec2& rhs)const
	{
		return x == rhs.x&&y == rhs.y;
	}
	Vec2 operator+(const Vec2& rhs)const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	Vec2& operator+=(const Vec2& rhs)const
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2 operator-(const Vec2& rhs)const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	Vec2& operator-=(const Vec2& rhs)const
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	template<class U>
	Vec2<T>& operator *=(const U& rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	template<class U>
	Vec2<T>& operator /=(const U& rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	T dot(const Vec2& rhs)
	{
		return x*rhs.x + y*rhs.y;
	}
	T cross(const Vec2& rhs)
	{
		return x*rhs.y - rhs.x*y;
	}
	T distance(const Vec2& rhs)
	{
		return(sqrt(rhs.x - x)*(rhs.x - x) + (rhs.y - y)*(rhs.y - y));
	}
};
template<class T, class U>
Vec2<T> operator *(const Vec2<T>& lhs, const U& rhs)
{
	return(Vec2<T>(lhs.x*rhs, lhs.y*rhs));
}
template<class T, class U>
Vec2<T> operator *(const U& lhs, const Vec2<T>& rhs)
{
	return(Vec2<T>(lhs*rhs.x, lhs*rhs.y));
}
template<class T, class U>
Vec2<T> operator /(const Vec2<T>& lhs, const U& rhs)
{
	return(Vec2<T>(lhs.x / rhs, lhs.y / rhs));
}
template<class T, class U>
Vec2<T> operator /(const U& lhs, const Vec2<T>& rhs)
{
	return(Vec2<T>(lhs / rhs.x, lhs / rhs.y));
}
using Vec2i = Vec2<int>;
using Vec2u = Vec2<size_t>;
using Vec2ll = Vec2<long long>;
using Vec2d = Vec2<double>;
using Vec2f = Vec2<float>;