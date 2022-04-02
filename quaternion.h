// quaternion.h

#ifndef _QUATERNION_h
#define _QUATERNION_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

struct ZYXRPYDeg { //RYP��
	float roll; //��X��
	float pitch; //��Y��
	float yaw; //��Z��
};

struct vec3 { //��ά����
	double x;
	double y;
	double z;
};

vec3 crossProduct(vec3 left, vec3 right); //�������
vec3 addVec3(vec3 left, vec3 right); //�������
vec3 scaleVec3(vec3 v, float coef); //����
float lengthVec3(vec3 v);

class Quaternion {
protected:
	double a, b, c, d; //a + bi + cj + dk;
	
public:
	Quaternion() {};
	ZYXRPYDeg getRPYDeg(); //����Ԫ��������̬��
	float fRad2Deg = 57.295779513f; //����ת�Ƕ�
	Quaternion(double a,double b, double c,double d): a(a),b(b),c(c),d(d) {}
	void setQuaternion(double _a, double _b, double _c, double _d);
	void diaplayQuaternion();
	void normalize(); //��������й�һ��
	static Quaternion mutiple(Quaternion qleft, Quaternion qright); //��Ԫ����˶�Ӧ������ת�ĵ���, right�ȱ任��left��任
};

#endif

