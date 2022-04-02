// MPU6050Filter.h
// 滤波器，负责MPU6050的数据读取和初步处理
// 包含一个I2C通信接口：
//		ReadAccGyr: 一次从MPU6050中读取7个值，修正固定误差
//		ReadAccGyrUnFixed: 无修正，原始数据
// 包含一个偏差校准方法
//		Calibration: 校准加速度计和陀螺仪的固定误差
// 包含一个滤波器：
//		filter: 暂时不需要滤波器

#ifndef _MPU6050FILTER_h
#define _MPU6050FILTER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <SoftwareSerial.h>

const int blueToothRX = 11;
const int blueToothTX = 12;

class MPU6050FilterClass
{
protected:
	const int MPU = 0x68; //MPU-6050的I2C地址
	const int nValCnt = 7; //一次读取寄存器的数量
	int dataPos = 0; //记录当前读取到了第几个数据
	//float standardG = 16384.0f;
	float standardG = 15899.0f; //注意，加速度计的输出范围是16384,但是本地G是15899

	/*固定误差校准*/
	int calibData[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; //校准数据
	const int nCalibTimes = 600; //校准时读数的次数

public:
	float gyr2Deg = 0.0; //根据角度范围不同，确定转换系数
	void init(int FS_SEL); //设定两种滤波模式下进行算数平均的采样数，不超过8
	MPU6050FilterClass() {};
	void ReadAccGyr(int * pVals);
	void Calibration();
	void ReadAccGyrUnFixed(bool isAddMode, int * pVals);//直接读取原始数据，没有校准，没有滤波
	void getOriginData(int *nVal); //获取滤波后的原始数据，已加上校准
};

extern MPU6050FilterClass filter;
extern SoftwareSerial BTSerial;
#endif
