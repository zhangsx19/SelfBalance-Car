#include "PID.h"

void PIDClass::PIDsetup(int dir1, int dir2, int stp1, int stp2, int slp1, int slp2) {
	dirpin_left = dir1;
	dirpin_right = dir2;
	stepperpin_left = stp1;
	stepperpin_right = stp2;
	sleep_left = slp1;
	sleep_right = slp2;
	//电机初始化
	pinMode(dirpin_left, OUTPUT);
	pinMode(stepperpin_left, OUTPUT);
	pinMode(dirpin_right, OUTPUT);
	pinMode(stepperpin_right, OUTPUT);
	//参数初始化
	distanceDelta = 0.0f;
	distanceSum = 0.0f;
	lastSpeed = 0.0f;
	//电机控制参数
	motorCoef = 0.155f; //电机控制（速度环）的输出系数
	//PID参数
	PIDv.Kp = 28.0f;
	PIDv.Ki = 0.04f;
	PIDv.Kd = 1.0f;

	PIDs.Kp = 4.0f;
	PIDs.Ki = 0.004f;
	PIDs.Kd = 0.16f;
	//初始化
	PIDv.lastTime = 0.0f;
	PIDv.turnLSpeed_Need = 0.0f;
	PIDv.turnRSpeed_Need = 0.0f;
	PIDv.errSum = 0.0f;
	PIDv.lastErr = 0.0f;
	PIDv.Setpoint = 0.0f;

	PIDs.lastTime = 0.0f;
	PIDs.Setpoint = 0.0f;
	PIDs.errSum = 0.0f;
	PIDs.lastErr = 0.0f;
	PIDs.output = 0.0f;
}

//算法执行
void PIDClass::PIDLoop()
{
	PIDv.TimeChange = (millis() - PIDv.lastTime);
	if (PIDv.TimeChange >= SampleTime)
	{
		PIDv.Input = MPU6050DMP.getAngle().roll;
		if (PIDv.Input > 50.0f || PIDv.Input < -50.0f) {
			digitalWrite(sleep_left, LOW);
			digitalWrite(sleep_right, LOW);
			BTSerial.println("-S");
			exit(0);
		}
		signalDetect(); //检测信号
		PIDSpeed();
		PIDDistance();
		//执行
		digitalWrite(sleep_left, HIGH);
		digitalWrite(sleep_right, HIGH);
		speedControl(motorCoef * PIDv.left_output, motorCoef * PIDv.right_output);
	}
}

void PIDClass::PIDLoopStart()
{
	PIDv.lastTime = millis();
	PIDs.lastTime = millis();
}

void PIDClass::signalDetect()
{
	if ((isControlling && millis() > sampletime) ||breakJudge) {//到目标点或中途停止
		PIDv.turnLSpeed_Need = 0.0f;
		PIDv.turnRSpeed_Need = 0.0f;//转弯
		PIDv.errSum = 0.0f;
		PIDs.Setpoint = 0.0f;
		PIDv.Setpoint = 0.0f;
		PIDv.Kp = 28.0f;
		PIDv.Ki = 0.04f;
		PIDv.Kd = 1.0f;
		isControlling = false;
		isDistanceLogging = true;
		distanceSum = 0.0f;
		turnSum = 0.0f;
	}
	else {
		if (BTSerial.available()) {
			char chrdir[5]; breakJudge = false;
			for (int i = 0; i < 5; i++)
			{
				chrdir[i] =(char) BTSerial.read();
			}
			char dis[3] = { chrdir[2] , chrdir[3] , chrdir[4] };
			switch (chrdir[0])
			{
			case 'L':
				PIDv.turnLSpeed_Need = -turningSpeed;
				PIDv.turnRSpeed_Need = turningSpeed;
				PIDs.Setpoint = atoi(dis);
				break;
			case 'R':
				PIDv.turnLSpeed_Need = turningSpeed;
				PIDv.turnRSpeed_Need = -turningSpeed;
				PIDs.Setpoint = atoi(dis);
				break;
			case 'F': //forward
				PIDv.turnLSpeed_Need = 0.0f;
				PIDv.turnRSpeed_Need = 0.0f;
				PIDs.Setpoint = atoi(dis) / 10.0f;
				isDistanceLogging = false;
				break;
			case 'B': //forward
				PIDv.turnLSpeed_Need = 0.0f;
				PIDv.turnRSpeed_Need = 0.0f;
				PIDs.Setpoint = -atoi(dis);
				BTSerial.print(PIDs.Setpoint);
				isDistanceLogging = false;
				break;
			case 'S': //Abort
				digitalWrite(sleep_left, LOW);
				digitalWrite(sleep_right, LOW);
				char stopNum[4]= { chrdir[1], chrdir[2] , chrdir[3] , chrdir[4] };
				if (stopNum=="0000")
				{
					delay(10000);
					break;
				}
				else
				{
					exit(0);
					break;
				}
			default:
				return;
			}
			switch (chrdir[1])
			{
			case '0'://低速
				PIDv.Setpoint = 0.0f;
				PIDv.Kp = 28.0f;
				PIDv.Ki = 0.04f;
				PIDv.Kd = 1.0f;
				break;
			case '1':
				PIDv.Setpoint = 1.0f;
				PIDv.Kp = 36.0f;
				PIDv.Ki = 0.06f;
				PIDv.Kd = 2.0f;
				break;
			default:
				return;
			}
			if (chrdir[0]=='F'|| chrdir[0] == 'B')
			{
				while (abs(PIDs.Setpoint * 10.0f - distanceSum) > 0.05*abs(PIDs.Setpoint * 10.0f) ) {//cm
					if (BTSerial.available())
					{
						BTSerial.print("-E1");
						breakJudge = true;
						break;
					}
					if (abs(PIDs.Setpoint * 10.0f - distanceSum) > 0.4*abs(PIDs.Setpoint * 10.0f))
					{
						PIDv.Setpoint = 0.0f;
						PIDv.Kp = 28.0f;
						PIDv.Ki = 0.04f;
						PIDv.Kd = 1.0f;
					}
					MPU6050DMP.GyroLoop();
					PIDv.TimeChange = (millis() - PIDv.lastTime);
					if (PIDv.TimeChange > sampletime)
					{
						
						PIDv.Input = MPU6050DMP.getAngle().roll;
						if (PIDv.Input > 50 || PIDv.Input < -50) {
							digitalWrite(sleep_left, LOW);
							digitalWrite(sleep_right, LOW);
							BTSerial.println("-S");
							exit(0);
						}
						PIDSpeed();
						PIDDistance();
						//执行
						digitalWrite(sleep_left, HIGH);
						digitalWrite(sleep_right, HIGH);
						speedControl(motorCoef * PIDv.left_output, motorCoef * PIDv.right_output);
						/*BTSerial.print("-");
						BTSerial.print(chrdir[0]);
						BTSerial.println(distanceDelta/1000);*/
						/*long now = millis();*/
					}
				}
			}
			else if (chrdir[0] == 'L' || chrdir[0] == 'R')
			{
				while (abs(abs(PIDs.Setpoint) - abs(turnSum)) > 0.05*abs(PIDs.Setpoint) ) {//cm
					if (BTSerial.available())
					{
						BTSerial.print("-E1");
						breakJudge = true;
						break;
					}
					MPU6050DMP.GyroLoop();
					PIDv.TimeChange = (millis() - PIDv.lastTime);
					if (PIDv.TimeChange > sampletime)
					{
						
						PIDv.Input = MPU6050DMP.getAngle().roll;
						if (PIDv.Input > 50 || PIDv.Input < -50) {
							digitalWrite(sleep_left, LOW);
							digitalWrite(sleep_right, LOW);
							BTSerial.println("-S");
							exit(0);
						}
						PIDSpeed();
						PIDDistance();
						//执行
						digitalWrite(sleep_left, HIGH);
						digitalWrite(sleep_right, HIGH);
						speedControl(motorCoef * PIDv.left_output, motorCoef * PIDv.right_output);
						/*BTSerial.print("-");
						BTSerial.print(chrdir[0]);
						BTSerial.println(turnDelta/1000);*/
						/*long now = millis();*/
					}
				}
			}
			if (!breakJudge)
			{
				BTSerial.print("-E0");
			}
			BTSerial.print("youzhiling");
			isControlling = true; //开启控制
		}
		
	}
}

void PIDClass::PIDSpeed()
{
	PIDv.error = PIDv.Setpoint - PIDv.Input;                     // 偏差值
	PIDv.errSum += PIDv.error * PIDv.TimeChange;
	PIDv.dErr = (PIDv.error - PIDv.lastErr) / PIDv.TimeChange;
	PIDv.output = PIDv.Kp * PIDv.error + PIDv.Ki * PIDv.errSum + PIDv.Kd * PIDv.dErr;// 计算输出值
	PIDv.output = -PIDv.output; //角度为正，电机应当正向运转进行补偿
	PIDv.left_output = PIDv.output + PIDv.turnLSpeed_Need;//左电机
	PIDv.right_output = PIDv.output + PIDv.turnRSpeed_Need;//右电机
	PIDv.lastErr = PIDv.error;
	PIDv.lastTime = millis();// 记录本次时间
	turnDelta=turningSpeed * PIDv.TimeChange;
	distanceDelta = lastSpeed * PIDv.TimeChange; //积分得到路程
	turnSum += turnDelta/1000;
	distanceSum += distanceDelta/1000;
	lastSpeed = PIDv.output;
}

void PIDClass::PIDDistance()
{
	PIDs.TimeChange = (millis() - PIDs.lastTime);
	PIDs.Input = distanceDelta / 10000.0f;// 输入赋值
	PIDs.error = PIDs.Setpoint - PIDs.Input;// 偏差值
	if (isDistanceLogging) PIDs.errSum += PIDs.error * PIDs.TimeChange; //如果不在人为控制状态才进行距离校准
	PIDs.dErr = (PIDs.error - PIDs.lastErr) / PIDs.TimeChange;
	PIDs.output = PIDs.Kp * PIDs.error + PIDs.Ki * PIDs.errSum + PIDs.Kd * PIDs.dErr;
	PIDv.Setpoint = PIDs.output;
	PIDs.lastErr = PIDs.error;
	PIDs.lastTime = millis(); // 记录本次时间
}

//电机前进后退控制
void PIDClass::speedControl(float speedL, float speedR) {
	float L, R;
	if (speedL < 0.0f) {
		digitalWrite(dirpin_left, 0);
		L = -speedL;
	}
	else {
		digitalWrite(dirpin_left, 1);
		L = speedL;
	}
	if (speedR < 0.0f) {
		digitalWrite(dirpin_right, 1);
		R = -speedR;
	}
	else {
		digitalWrite(dirpin_right, 0);
		R = speedR;
	}
	pwm(L, R);
}

void PIDClass::pwm(float leftSpeed, float rightSpeed)
{
	//超过限制关闭pwm
	if (leftSpeed < 0.16f || rightSpeed < 0.16f || leftSpeed > 109.1f || rightSpeed > 109.1f) {
		TCCR1A = _BV(WGM11) | _BV(WGM10);
		TCCR2A = _BV(WGM21) | _BV(WGM20);
		return;
	}
	LPreScaler = 1024;
	RPreScaler = 1024;
	//开启pwm
	TCCR1A = _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
	TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
	//左侧电机
	if (leftSpeed > 0.615) { //设计if分支使得低速状态反应比较灵活
		if (leftSpeed < 2.5) {
			LPreScaler = 256;
			TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);
		}
		else {
			if (leftSpeed < 22.5) {
				LPreScaler = 64;
				TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11) | _BV(CS10);
			}
			else {
				LPreScaler = 8;
				TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS11);
			}
		}
	}
	else { //1024
		TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12) | _BV(CS10);
	}
	//右侧电机
	if (rightSpeed > 0.615) {
		if (rightSpeed < 2.5) {
			RPreScaler = 256;
			TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS21);
		}
		else {
			if (rightSpeed < 22.5) {
				RPreScaler = 64;
				TCCR2B = _BV(WGM22) | _BV(CS22);
			}
			else {
				RPreScaler = 8;
				TCCR2B = _BV(WGM22) | _BV(CS21);
			}
		}
	}
	else { //1024
		TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS21) | _BV(CS20);
	}
	OCR1A = (39268.7f / ((float)LPreScaler * leftSpeed));
	OCR2A = (39268.7f / ((float)RPreScaler * rightSpeed));
}

PIDClass PID;