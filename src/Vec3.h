#pragma once


#include <algorithm>
#include "mathHelpers.h"

template <typename T>
class _Vec3 
{
public:
	_Vec3() = default;
	_Vec3(T x, T y, T z)
		:
		x(x),
		y(y),
		z(z)
	{}
	_Vec3(T v)
		:
		x(v),
		y(v),
		z(v)
	{}
	template <typename T2>
	explicit operator _Vec3<T2>() const
	{
		return{ (T2)x,(T2)y,(T2)z };
	}
	T		LenSq() const
	{
		return (x*x + y*y + z*z);
	}
	T		Len() const
	{
		return sqrt(LenSq());
	}
	_Vec3&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}
	_Vec3&	Clamp()
	{
		x = CLAMP(x, 0.0f, 1.0f);
		y = CLAMP(y, 0.0f, 1.0f);
		z = CLAMP(z, 0.0f, 1.0f);
		return *this;
	}

	_Vec3	Abs() const
	{
		return { abs(x), abs(y), abs(z) };
	}
	_Vec3	GetNormalized() const
	{
		_Vec3 norm = *this;
		norm.Normalize();
		return norm;
	}
	_Vec3	operator-() const
	{
		return _Vec3(-x, -y, -z);
	}
	_Vec3&	operator=(const _Vec3 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	_Vec3&	operator+=(const _Vec3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	_Vec3&	operator+=(const T &rhs)
	{
		x += rhs;
		y += rhs;
		z += rhs;
		return *this;
	}
	_Vec3&	operator-=(const _Vec3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	_Vec3&	operator-=(const T &rhs)
	{
		x -= rhs;
		y -= rhs;
		z -= rhs;
		return *this;
	}
	T		operator*(const _Vec3 &rhs) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	_Vec3	operator+(const _Vec3 &rhs) const
	{
		return _Vec3(*this) += rhs;
	}
	_Vec3	operator+(const T &rhs) const
	{
		return _Vec3(*this) += rhs;
	}
	_Vec3	operator-(const T &rhs) const
	{
		return _Vec3(*this) -= rhs;
	}
	_Vec3	operator-(const _Vec3 &rhs) const
	{
		return _Vec3(*this) -= rhs;
	}
	_Vec3&	operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	_Vec3	operator*(const T &rhs) const
	{
		return _Vec3(*this) *= rhs;
	}
	_Vec3	operator%(const _Vec3& rhs) const
	{
		return _Vec3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x);
	}
	_Vec3&	operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	_Vec3	operator/(const T &rhs) const
	{
		return _Vec3(*this) /= rhs;
	}
	bool	operator==(const _Vec3 &rhs) const
	{
		return x == rhs.x && y == rhs.y && rhs.z == z;
	}
	bool	operator!=(const _Vec3 &rhs) const
	{
		return !(*this == rhs);
	}
	// clamp to between 0.0 ~ 1.0
	_Vec3&	Saturate()
	{
		x = std::min(1.0f, std::max(0.0f, x));
		y = std::min(1.0f, std::max(0.0f, y));
		z = std::min(1.0f, std::max(0.0f, z));
		return *this;
	}
	// clamp to between 0.0 ~ 1.0
	_Vec3	GetSaturated() const
	{
		_Vec3 temp(*this);
		temp.Saturate();
		return temp;
	}
	// x3 = x1 * x2 etc.
	_Vec3&  Hadamard(const _Vec3& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		return *this;
	}
	// x3 = x1 * x2 etc.
	_Vec3	GetHadamard(const _Vec3& rhs) const
	{
		_Vec3 temp(*this);
		temp.Hadamard(rhs);
		return temp;
	}

	static _Vec3 getRandom() {
		// returns random vector between -1 and 1 for each axis
		_Vec3 v;
		v.x = (random16() - 32768) / 32768.0f;
		v.y = (random16() - 32768) / 32768.0f;
		v.z = (random16() - 32768) / 32768.0f;
		return v;
	}
public:
	T x;
	T y;
	T z;
};

typedef _Vec3<float> Vec3;
typedef _Vec3<double> Ved3;
typedef _Vec3<int> Vei3;

Vec3&	cos(Vec3 &v)
{
	v.x = cos(v.x);
	v.y = cos(v.y);
	v.z = cos(v.z);
	return v;
}