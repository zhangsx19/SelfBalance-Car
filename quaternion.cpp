// 四元数实现
//
//

#include "quaternion.h"
#include <math.h>
ZYXRPYDeg Quaternion::getRPYDeg()
{
	//由四元数得到RPY角，旋转顺序是Z-Y-X, 并转为角度
	return ZYXRPYDeg{
		fRad2Deg * (atan2(2.0f*(a * b + c * d),1.0f - 2.0f * (b * b + c * c))),
		fRad2Deg * (asin(2.0f * (a * c - b * d))),
		fRad2Deg * (atan2(2.0f * (a * d + b * c), 1.0f - 2.0f * (c * c + d * d)))
	};
}

void Quaternion::setQuaternion(double _a, double _b, double _c, double _d)
{
	a = _a; b = _b; c = _c; d = _d;
}

void Quaternion::diaplayQuaternion()
{
}


void Quaternion::normalize()
{
	double len = sqrt(a * a + b * b + c * c + d * d);
	if (abs(len) < 0.005 || abs(len - 1.0f) < 0.005) return;
	a = a / len;
	b = b / len;
	c = c / len;
	d = d / len;
}

Quaternion Quaternion::mutiple(Quaternion qleft, Quaternion qright)
{
	//四元数相乘
	return Quaternion(
		qleft.a * qright.a - qleft.b * qright.b - qleft.c * qright.c - qleft.d * qright.d,
		qleft.b * qright.a + qleft.a * qright.b - qleft.d * qright.c + qleft.c * qright.d,
		qleft.c * qright.a + qleft.d * qright.b + qleft.a * qright.c - qleft.b * qright.d,
		qleft.d * qright.a - qleft.c * qright.b + qleft.b * qright.c + qleft.a * qright.d
	);
}

vec3 crossProduct(vec3 left, vec3 right)
{
	return vec3{
		left.y * right.z - left.z * right.y,
		left.z * right.x - left.x * right.z,
		left.x * right.y - left.y * right.x
	};
}

vec3 addVec3(vec3 left, vec3 right)
{
	return vec3{
	left.x + right.x,
	left.y + right.y,
	left.z + right.z 
	};
}

vec3 scaleVec3(vec3 v, float coef)
{
	return vec3{
		v.x * coef,
		v.y * coef,
		v.z * coef
	};
}

float lengthVec3(vec3 v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
