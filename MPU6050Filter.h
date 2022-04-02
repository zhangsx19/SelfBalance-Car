// MPU6050Filter.h
// �˲���������MPU6050�����ݶ�ȡ�ͳ�������
// ����һ��I2Cͨ�Žӿڣ�
//		ReadAccGyr: һ�δ�MPU6050�ж�ȡ7��ֵ�������̶����
//		ReadAccGyrUnFixed: ��������ԭʼ����
// ����һ��ƫ��У׼����
//		Calibration: У׼���ٶȼƺ������ǵĹ̶����
// ����һ���˲�����
//		filter: ��ʱ����Ҫ�˲���

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
	const int MPU = 0x68; //MPU-6050��I2C��ַ
	const int nValCnt = 7; //һ�ζ�ȡ�Ĵ���������
	int dataPos = 0; //��¼��ǰ��ȡ���˵ڼ�������
	//float standardG = 16384.0f;
	float standardG = 15899.0f; //ע�⣬���ٶȼƵ������Χ��16384,���Ǳ���G��15899

	/*�̶����У׼*/
	int calibData[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f }; //У׼����
	const int nCalibTimes = 600; //У׼ʱ�����Ĵ���

public:
	float gyr2Deg = 0.0; //���ݽǶȷ�Χ��ͬ��ȷ��ת��ϵ��
	void init(int FS_SEL); //�趨�����˲�ģʽ�½�������ƽ���Ĳ�������������8
	MPU6050FilterClass() {};
	void ReadAccGyr(int * pVals);
	void Calibration();
	void ReadAccGyrUnFixed(bool isAddMode, int * pVals);//ֱ�Ӷ�ȡԭʼ���ݣ�û��У׼��û���˲�
	void getOriginData(int *nVal); //��ȡ�˲����ԭʼ���ݣ��Ѽ���У׼
};

extern MPU6050FilterClass filter;
extern SoftwareSerial BTSerial;
#endif
