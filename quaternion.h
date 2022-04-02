// quaternion.h

#ifndef _QUATERNION_h
#define _QUATERNION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

struct ZYXRPYDeg { //RYP角
	float roll; //绕X轴
	float pitch; //绕Y轴
	float yaw; //绕Z轴
};

struct vec3 { //三维向量
	double x;
	double y;
	double z;
};

vec3 crossProduct(vec3 left, vec3 right); //向量叉乘
vec3 addVec3(vec3 left, vec3 right); //向量相加
vec3 scaleVec3(vec3 v, float coef); //向量
float lengthVec3(vec3 v);

class Quaternion {
protected:
	double a, b, c, d; //a + bi + cj + dk;
	
public:
	Quaternion() {};
	ZYXRPYDeg getRPYDeg(); //由四元数导出姿态角
	float fRad2Deg = 57.295779513f; //弧度转角度
	Quaternion(double a,double b, double c,double d): a(a),b(b),c(c),d(d) {}
	void setQuaternion(double _a, double _b, double _c, double _d);
	void diaplayQuaternion();
	void normalize(); //对自身进行归一化
	static Quaternion mutiple(Quaternion qleft, Quaternion qright); //四元数相乘对应两个旋转的叠加, right先变换，left后变换
};

#endif

