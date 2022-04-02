// PID.h

#ifndef _PID_h
#define _PID_h
#include "MPU6050DMP.h"

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class PIDClass {
protected:
	struct PIDParameterV // static ���Ǳ���
	{
		long lastTime;           // ǰ��ʱ��
		float left_output, right_output;
		float Input, output, Setpoint, error, errSum, dErr, lastErr, TimeChange;
		float turnLSpeed_Need, turnRSpeed_Need;// ת������
		float Kp, Ki, Kd;                    // ����ϵ��(8.0f)������ϵ��(0.05)��΢��ϵ��(0.26)
	};
	struct PIDParameterS // static ���Ǳ���
	{
		long lastTime;           // ǰ��ʱ��
		float Input, output, Setpoint, error, errSum, dErr, lastErr, TimeChange;
		float Kp, Ki, Kd;                    // ����ϵ��(8.0f)������ϵ��(0.05)��΢��ϵ��(0.26)
	};


	/*�������*/
	int dirpin_left;
	int stepperpin_left;
	int dirpin_right;
	int stepperpin_right;
	int sleep_left;
	int sleep_right;
	int LPreScaler = 1024; //Ԥ���������ڿ���pwm
	int RPreScaler = 1024;
	/*********** PID���������� *********/
	PIDParameterV PIDv;
	PIDParameterS PIDs;
	float motorCoef; //������ת������
	float accCoef; //���ٶ�ת������
	long signalExecTime = 0;//�ź�ִ�е�ʱ�䣨ms��
	float turningSpeed = 22.0f; //ת��ʱ�ĸ����ٶ�
	float lastSpeed = 0.0f; //���ھ��������ļ���
	float distanceDelta = 0.0f;
	float turnDelta = 0.0f;
	float distanceSum = 0.0f;
	float turnSum = 0.0f;
	float sampletime = 0.2f;
	bool breakJudge = false;
	bool isControlling = false;
	bool isDistanceLogging = true;
	int SampleTime = 20; //PID���ƵĲ������ms
	void signalDetect(); //����������͵��ź�
	void PIDSpeed();
	void PIDDistance();
	void speedControl(float speedL, float speedR);
	void pwm(float leftSpeed, float rightSpeed); //�����speed�����ӵ����ٶȣ���λ��cm/s
public:
	void PIDsetup(int dir1, int dir2, int stp1, int stp2, int slp1, int slp2);
	void PIDLoop();
	void PIDLoopStart(); //���뾡����PIDLoopǰ����
};

extern PIDClass PID;
extern SoftwareSerial BTSerial;
#endif

