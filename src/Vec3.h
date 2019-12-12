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
		return (x*x + y * y + z * z);
	}
	T		Len() const
	{
		return sqrt(LenSq());
	}
	_Vec3&	Normalize()
	{
		const T length = Len();
		if (length != 0) {
			x /= length;
			y /= length;
			z /= length;
		}
		return *this;
	}
	_Vec3&	Limit(T l)
	{
		const T length = Len();
		if (length > l) {
			x /= length;
			y /= length;
			z /= length;
			*this *= l;
		}
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


Vec3 rgb2lab(Vec3 rgb) {
	float r, g, b, x, y, z;
	r = rgb.x / 255.0f;
	g = rgb.y / 255.0f;
	b = rgb.z / 255.0f;

	r = (r > 0.04045f) ? pow((r + 0.055f) / 1.055f, 2.4f) : r / 12.92f;
	g = (g > 0.04045f) ? pow((g + 0.055f) / 1.055f, 2.4f) : g / 12.92f;
	b = (b > 0.04045f) ? pow((b + 0.055f) / 1.055f, 2.4f) : b / 12.92f;

	x = (r * 0.4124f + g * 0.3576f + b * 0.1805f) / 0.95047f;
	y = (r * 0.2126f + g * 0.7152f + b * 0.0722f) / 1.00000f;
	z = (r * 0.0193f + g * 0.1192f + b * 0.9505f) / 1.08883f;

	x = (x > 0.008856f) ? cbrt(x) : (7.787f * x) + 16.0f / 116.0f;
	y = (y > 0.008856f) ? cbrt(y) : (7.787f * y) + 16.0f / 116.0f;
	z = (z > 0.008856f) ? cbrt(z) : (7.787f * z) + 16.0f / 116.0f;

	return Vec3((116.0f * y) - 16, 500 * (x - y), 200 * (y - z));

}


Vec3& lab2rgb(Vec3& lab) {
	float y = (lab.x + 16.0f) / 116.0f;
	float x = lab.y / 500.0f + y;
	float z = y - lab.z / 200.0f;
	float r, g, b;

	float x3 = x * x*x;
	float y3 = y * y*y;
	float z3 = z * z*z;

	x = 0.95047f * ((x3 > 0.008856f) ? x3 : (x - 16.0f / 116.0f) / 7.787f);
	y = 1.00000f * ((y3 > 0.008856f) ? y3 : (y - 16.0f / 116.0f) / 7.787f);
	z = 1.08883f * ((z3 > 0.008856f) ? z3 : (z - 16.0f / 116.0f) / 7.787f);

	r = x * 3.2406f + y * -1.5372f + z * -0.4986f;
	g = x * -0.9689f + y * 1.8758f + z * 0.0415f;
	b = x * 0.0557f + y * -0.2040f + z * 1.0570f;

	r = (r > 0.0031308) ? (1.055 * pow(r, 1 / 2.4) - 0.055) : 12.92 * r;
	g = (g > 0.0031308) ? (1.055 * pow(g, 1 / 2.4) - 0.055) : 12.92 * g;
	b = (b > 0.0031308) ? (1.055 * pow(b, 1 / 2.4) - 0.055) : 12.92 * b;

	lab.x = max(0.0f, min(1.0f, r)) * 255;
	lab.y = max(0.0f, min(1.0f, g)) * 255;
	lab.z = max(0.0f, min(1.0f, b)) * 255;
	return lab;
}

Vec3& lab2lch(Vec3& lab) {
	float l = lab.x,
		a = lab.y,
		b = lab.z,
		c, h;
	c = sqrt(a*a + b * b);
	h = atan2(b, a);
	h = h / PI * 180;
	if (h < 0) {
		h += 360;
	}
	else if (h >= 360) {
		h -= 360;
	}
	lab.x = l;
	lab.y = c;
	lab.z = h;
	return lab;
}

Vec3& lch2lab(Vec3& lch) {
	float l = lch.x,
		c = lch.y,
		h = (lch.z * PI) / 180.0f,
		a, b;
	a = c * cos(h);
	b = c * sin(h);
	lch.x = l;
	lch.y = a;
	lch.z = b;
	return lch;
}



Vec3& rgb2lch(Vec3& rgb) {
	rgb = rgb2lab(rgb);
	rgb = lab2lch(rgb);
	return rgb;
}

Vec3& lch2rgb(Vec3& lch) {
	lch = lch2lab(lch);
	lch = lab2rgb(lch);
	return lch;
}