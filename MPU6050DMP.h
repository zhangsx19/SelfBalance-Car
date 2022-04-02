// MPU6050DMP.h
// ������ƽ�⳵����̬����
// ����һ����̬���㷽����
//		quaternionUpdateStep: ���ڵ�Чת��ʸ���Ľ��ٶȻ����㷨
// ����������̬������������
//		gyroCalibration: ����Ԥ��ģ�͵��ۼ���������㷨
//		accUpdateStep: ���ڼ��ٶȼƵ�Roll�������㷨
// ����һ�����Է�����
//		gryoTestSampling: ��������ָ��������ԭʼ���ݲ������������ݷ���
#ifndef _MPU6050DMP_h
#define _MPU6050DMP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Math.h>
#include "MPU6050Filter.h"

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

class MPU6050DMPClass
{
protected:

	/*����Ƶ��*/
	int sampleDelay = 0; //Ĭ��ȫ�ٲ���
	/*��̬����*/
	float fRad2Deg = 57.295779513f; //������תΪ�Ƕȵĳ���
	float gyr2Deg = 0.0f;
	int filterData[7]; //��filter�ж���������
	unsigned long nLastTime = 0; //��һ�ζ�����ʱ��
	void integralUpdateStep();
	ZYXRPYDeg integralAngle{ 0.0f,0.0f,0.0f };
	/*�ۼ��������*/
	float secondFixTime = 30000.0f; //�ڶ���������������ʱ�䣬��λΪ����
	vec3 fixAngle{ 0.0f,0.0f,0.0f }; //��Ŷ���������ŷ���ǣ����10s���ۼ����
	long startLoopMills = 0; //��һ�������ĺ�����
	void gyroCalibration(); //��������

	/*���ڼ��ٶȵ���Ư����*/
	bool isAccFixEnabled = false; //�Ƿ������ڼ��ٶȵ�����
	float localG = 15899.0f; //����Gֵ
	long lastAccFixTime = 0;
	const long accFixPeriod = 500; //ÿ��һ��ʱ�����һ����������
	void accFixStep(); //�ڵͽ��ٶ�ʱ���ü��ٶȽ�������

	/*�����Իص�����*/
	int nowRound = 0;
	int targetRound = 40;
	long lastActionMillis = 0; //��һ��ִ�е�ʱ��
	void periodicAction(); //������ִ�к���
public:
	MPU6050DMPClass() {};
	void init();
	/*�����ǲ���*/
	void GyroTestSampling(int sampleCount, int delayMills, int *data); //���������������źŷ���
	void display(); //�ڵ���gyroLoop֮�󣬽���������ʾ
	//����ӿ�
	float cosG = 0.0f;
	void GyroSetup(int _sampleDelay); //��ʼ��
	void GyroLoop(); //���ô���Խ��
	void GyroLoopStart(bool _isAccFixEnabled); //ʱ������ú�����һ��Ҫ��whileѭ����ʼ֮ǰ���ã��Ҿ�����Ҫ�м����
	void setAngle(float roll, float pitch, float yaw);
	ZYXRPYDeg getAngle(); //�������չ��Ƶ�ŷ����
};

extern MPU6050DMPClass MPU6050DMP;
extern SoftwareSerial BTSerial;

#endif
