// MPU6050DMP.h
// 用于自平衡车的姿态测算
// 包含一种姿态解算方法：
//		quaternionUpdateStep: 基于等效转动矢量的角速度积分算法
// 包含两种姿态角修正方法：
//		gyroCalibration: 基于预测模型的累计误差修正算法
//		accUpdateStep: 基于加速度计的Roll角修正算法
// 包含一个测试方法：
//		gryoTestSampling: 连续进行指定次数的原始数据采样，用于数据分析
#ifndef _MPU6050DMP_h
#define _MPU6050DMP_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <Math.h>
#include "MPU6050Filter.h"

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

class MPU6050DMPClass
{
protected:

	/*采样频率*/
	int sampleDelay = 0; //默认全速采样
	/*姿态估计*/
	float fRad2Deg = 57.295779513f; //将弧度转为角度的乘数
	float gyr2Deg = 0.0f;
	int filterData[7]; //从filter中读出的数据
	unsigned long nLastTime = 0; //上一次读数的时间
	void integralUpdateStep();
	ZYXRPYDeg integralAngle{ 0.0f,0.0f,0.0f };
	/*累计误差修正*/
	float secondFixTime = 30000.0f; //第二次修正采样的总时间，单位为毫秒
	vec3 fixAngle{ 0.0f,0.0f,0.0f }; //存放二次修正的欧拉角，检测10s的累计误差
	long startLoopMills = 0; //上一次修正的毫秒数
	void gyroCalibration(); //二次修正

	/*基于加速度的零漂修正*/
	bool isAccFixEnabled = false; //是否开启基于加速度的修正
	float localG = 15899.0f; //本地G值
	long lastAccFixTime = 0;
	const long accFixPeriod = 500; //每隔一段时间进行一次修正尝试
	void accFixStep(); //在低角速度时利用加速度进行修正

	/*周期性回调函数*/
	int nowRound = 0;
	int targetRound = 40;
	long lastActionMillis = 0; //上一次执行的时间
	void periodicAction(); //周期性执行函数
public:
	MPU6050DMPClass() {};
	void init();
	/*陀螺仪测试*/
	void GyroTestSampling(int sampleCount, int delayMills, int *data); //持续采样，用于信号分析
	void display(); //在调用gyroLoop之后，进行数据显示
	//对外接口
	float cosG = 0.0f;
	void GyroSetup(int _sampleDelay); //初始化
	void GyroLoop(); //调用次数越多
	void GyroLoopStart(bool _isAccFixEnabled); //时间戳设置函数，一定要在while循环开始之前调用，且尽量不要有间隔。
	void setAngle(float roll, float pitch, float yaw);
	ZYXRPYDeg getAngle(); //返回最终估计的欧拉角
};

extern MPU6050DMPClass MPU6050DMP;
extern SoftwareSerial BTSerial;

#endif
