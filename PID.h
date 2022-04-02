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
	struct PIDParameterV // static 不是必须
	{
		long lastTime;           // 前次时间
		float left_output, right_output;
		float Input, output, Setpoint, error, errSum, dErr, lastErr, TimeChange;
		float turnLSpeed_Need, turnRSpeed_Need;// 转弯设置
		float Kp, Ki, Kd;                    // 比例系数(8.0f)、积分系数(0.05)、微分系数(0.26)
	};
	struct PIDParameterS // static 不是必须
	{
		long lastTime;           // 前次时间
		float Input, output, Setpoint, error, errSum, dErr, lastErr, TimeChange;
		float Kp, Ki, Kd;                    // 比例系数(8.0f)、积分系数(0.05)、微分系数(0.26)
	};


	/*电机控制*/
	int dirpin_left;
	int stepperpin_left;
	int dirpin_right;
	int stepperpin_right;
	int sleep_left;
	int sleep_right;
	int LPreScaler = 1024; //预除数，用于控制pwm
	int RPreScaler = 1024;
	/*********** PID控制器参数 *********/
	PIDParameterV PIDv;
	PIDParameterS PIDs;
	float motorCoef; //电机输出转换参数
	float accCoef; //加速度转换参数
	long signalExecTime = 0;//信号执行的时间（ms）
	float turningSpeed = 22.0f; //转向时的附加速度
	float lastSpeed = 0.0f; //用于距离增量的计算
	float distanceDelta = 0.0f;
	float turnDelta = 0.0f;
	float distanceSum = 0.0f;
	float turnSum = 0.0f;
	float sampletime = 0.2f;
	bool breakJudge = false;
	bool isControlling = false;
	bool isDistanceLogging = true;
	int SampleTime = 20; //PID控制的采样间隔ms
	void signalDetect(); //检测蓝牙发送的信号
	void PIDSpeed();
	void PIDDistance();
	void speedControl(float speedL, float speedR);
	void pwm(float leftSpeed, float rightSpeed); //这里的speed是轮子的线速度，单位是cm/s
public:
	void PIDsetup(int dir1, int dir2, int stp1, int stp2, int slp1, int slp2);
	void PIDLoop();
	void PIDLoopStart(); //必须尽早在PIDLoop前调用
};

extern PIDClass PID;
extern SoftwareSerial BTSerial;
#endif

