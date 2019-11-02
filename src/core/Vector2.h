#pragma once
#include<cmath>
template<class T>
class Vec2
{
public:
	T x, y;
	Vec2() { x = T(), y = T(); }
	Vec2(const T& xx, const T& yy) :x(xx), y(yy) {}
	template <class U>
	Vec2(const Vec2<U>& tar) { x = tar.x; y = tar.y; }
	template <class U>
	bool operator ==(const Vec2<U>& rhs)const
	{
		return x == rhs.x&&y == rhs.y;
	}
	template <class U>
	bool operator !=(const Vec2<U>& rhs)const
	{
		return x != rhs.x||y != rhs.y;
	}
	template <class U>
	Vec2 operator+(const Vec2<U>& rhs)const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	template <class U>
	Vec2& operator+=(const Vec2<U>& rhs)const
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	template <class U>
	Vec2 operator-(const Vec2<U>& rhs)const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	template <class U>
	Vec2& operator-=(const Vec2<U>& rhs)const
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
	template <class U>
	T dot(const Vec2<U>& rhs)
	{
		return x*rhs.x + y*rhs.y;
	}
	template <class U>
	T cross(const Vec2<U>& rhs)
	{
		return x*rhs.y - rhs.x*y;
	}
	template <class U>
	T distance(const Vec2<U>& rhs)
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
using Vec2u = Vec2<unsigned int>;
using Vec2ll = Vec2<long long>;
using Vec2d = Vec2<double>;
using Vec2f = Vec2<float>;