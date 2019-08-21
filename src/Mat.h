/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Mat2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once

#include "Vec3.h"
//#include "Vec4.h"

template <typename T>
class _Mat
{
public:
	_Mat& operator=(const _Mat& rhs)
	{
		memcpy(elements, rhs.elements, sizeof(elements));
		return *this;
	}
	_Mat& operator*=(T rhs)
	{
		for (auto& row : elements)
		{
			for (T& e : row)
			{
				e *= rhs;
			}
		}
		return *this;
	}
	_Mat operator*(T rhs) const
	{
		_Mat result = *this;
		return result *= rhs;
	}
	_Mat& operator*=(const _Mat& rhs)
	{
		return *this = *this * rhs;
	}
	_Mat operator*(const _Mat& rhs) const
	{
		_Mat result;
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				T sum = (T)0.0;
				for (size_t i = 0; i < 3; i++)
				{
					sum += elements[j][i] * rhs.elements[i][k];
				}
				result.elements[j][k] = sum;
			}
		}
		return result;
	}
	_Mat operator!() const
	{
		_Mat xp;
		for (size_t j = 0; j < 3; j++)
		{
			for (size_t k = 0; k < 3; k++)
			{
				xp.elements[j][k] = elements[k][j];
			}
		}
		return xp;
	}

	constexpr static _Mat Identity()
	{

		return {
			(T)1.0,(T)0.0,(T)0.0,
			(T)0.0,(T)1.0,(T)0.0,
			(T)0.0,(T)0.0,(T)1.0,
		};
	}
	
	constexpr static _Mat Scaling(T factor)
	{

		return{
			factor,(T)0.0,(T)0.0,
			(T)0.0,factor,(T)0.0,
			(T)0.0,(T)0.0,factor,
		};
	}

	
	static _Mat RotationZ(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);

		return{
			 cosTheta, sinTheta, (T)0.0,
			-sinTheta, cosTheta, (T)0.0,
			(T)0.0,    (T)0.0,   (T)1.0,
		};
	}
	
	
	static _Mat RotationY(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return{
			 cosTheta, (T)0.0,-sinTheta,
			 (T)0.0,   (T)1.0, (T)0.0,
			 sinTheta, (T)0.0, cosTheta
		};
	}
		
	static _Mat RotationX(T theta)
	{
		const T sinTheta = sin(theta);
		const T cosTheta = cos(theta);
		return{
			(T)1.0, (T)0.0,   (T)0.0,
			(T)0.0, cosTheta, sinTheta,
			(T)0.0,-sinTheta, cosTheta,
		};
	}
	
	template<class V>
	constexpr static _Mat Translation(const V& tl)
	{
		return Translation(tl.x, tl.y, tl.z);
	}

	
	
	//constexpr static _Mat ProjectionHFOV(T fov, T ar, T n, T f)
	//{
	//	const auto fov_rad = fov * (T)PI / (T)180.0;
	//	const auto w = (T)1.0f / std::tan(fov_rad / (T)2.0);
	//	const auto h = w * ar;
	//	return {
	//		w,		(T)0.0,	(T)0.0,				(T)0.0,
	//		(T)0.0,	h,		(T)0.0,				(T)0.0,
	//		(T)0.0,	(T)0.0,	f / (f - n),		(T)1.0,
	//		(T)0.0,	(T)0.0,	-n * f / (f - n),	(T)0.0,
	//	};
	//}

	//static constexpr _Mat Projection(T w, T h, T n, T f) {
	//	return {
	//		(T)2.0 * n / w,	(T)0.0,			(T)0.0,				(T)0.0,
	//		(T)0.0,			(T)2.0 * n / h,	(T)0.0,				(T)0.0,
	//		(T)0.0,			(T)0.0,			f / (f - n),		(T)1.0,
	//		(T)0.0,			(T)0.0,			-n * f / (f - n),	(T)0.0,
	//	};
	//}

public:
	// [ row ][ col ]
	T elements[3][3];

};




template<typename T>
_Vec3<T>& operator*=(_Vec3<T>& lhs, const _Mat<T>& rhs)
{
	return lhs = lhs * rhs;
}

template<typename T>
_Vec3<T> operator*(const _Vec3<T>& lhs, const _Mat<T>& rhs)
{
	return{
		lhs.x * rhs.elements[0][0] + lhs.y * rhs.elements[1][0] + lhs.z * rhs.elements[2][0],
		lhs.x * rhs.elements[0][1] + lhs.y * rhs.elements[1][1] + lhs.z * rhs.elements[2][1],
		lhs.x * rhs.elements[0][2] + lhs.y * rhs.elements[1][2] + lhs.z * rhs.elements[2][2]
	};
}




typedef _Mat<float> Mat3;
typedef _Mat<double> Mad3;
