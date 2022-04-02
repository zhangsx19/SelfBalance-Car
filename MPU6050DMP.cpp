#include "MPU6050DMP.h"

MPU6050DMPClass MPU6050DMP;

void MPU6050DMPClass::periodicAction()
{
	if (nowRound < targetRound) {
		nowRound++;
		//�����ÿ�ֶ���Ҫ������
		accFixStep();
		return;
	}
	else {
		nowRound = 0;
		//�Զ������
		//display();
	}
	//����Ҫ�������
	lastActionMillis = millis();
}

void MPU6050DMPClass::init()
{
}

void MPU6050DMPClass::GyroTestSampling(int sampleCount, int delay_mills, int *data)
{
	int readouts[7];
	BTSerial.print("Sampling start, ms=");
	BTSerial.println(millis());
	for (int i = 1; i <= sampleCount; i++) {
		filter.ReadAccGyrUnFixed(false, readouts);
		//delayMicroseconds(delay_mills);
		//BTSerial.print(i);
		//BTSerial.print(",");
		//BTSerial.print(millis());
		for (int k = 0; k < 7; k++) {
			//BTSerial.print(",");
			//BTSerial.print(readouts[k]);
		}
		//BTSerial.println();
	}
	BTSerial.print("Sampling end, ms=");
	BTSerial.println(millis());
}

//ָ��������������Ϣ
void MPU6050DMPClass::display() {
	//��η�Ҫ��ʾ������
	ZYXRPYDeg angle = getAngle();
	BTSerial.print("F(Hz)=");
	BTSerial.print(targetRound * 1000.0f / (millis() - lastActionMillis));
	BTSerial.print("\t Roll:");
	BTSerial.print(angle.roll);
	BTSerial.print("\tYaw:");
	BTSerial.print(angle.yaw);
	BTSerial.println();
}

void MPU6050DMPClass::GyroSetup(int _sampleDelay)
{
	BTSerial.println("GyroSetup...");
	sampleDelay = _sampleDelay;
	filter.init(3); //���ÿ�����Ƕȣ�3=2000,2=1000,1=500,0=250
	gyr2Deg = filter.gyr2Deg;
	lastAccFixTime = millis();
	lastActionMillis = millis();
	filter.Calibration();
	isAccFixEnabled = false;
	nLastTime = micros();
	gyroCalibration();

}

void MPU6050DMPClass::GyroLoop()
{
	integralUpdateStep();
	periodicAction(); //������ִ�еĺ���
	delay(sampleDelay); //�ӳ�
}
void MPU6050DMPClass::GyroLoopStart(bool _isAccFixEnabled = true)
{
	//�������ٶ����������������м�ʱ��
	isAccFixEnabled = _isAccFixEnabled;
	long ms = millis();
	startLoopMills = ms;
	nLastTime = micros();
	lastActionMillis = ms;
	lastAccFixTime = ms;
}
void MPU6050DMPClass::setAngle(float roll, float pitch, float yaw)
{
	integralAngle = ZYXRPYDeg{ roll,pitch,yaw };
}
ZYXRPYDeg MPU6050DMPClass::getAngle()
{
	float fixCoef = (millis() - startLoopMills) / secondFixTime;
	return ZYXRPYDeg{
		integralAngle.roll - fixCoef * fixAngle.x,
		integralAngle.pitch - fixCoef * fixAngle.y,
		integralAngle.yaw - fixCoef * fixAngle.z,
	};
}

void MPU6050DMPClass::gyroCalibration()
{
	
	BTSerial.println("Running calibration: second part. Don't move the sensor.");
	//�����Ư
	startLoopMills = millis();
	nLastTime = micros();
	fixAngle = vec3{ 0,0,0 };
	long endms = millis() + (long)secondFixTime;
	while (millis() < endms) {
		GyroLoop();
	}
	//��¼��
	ZYXRPYDeg fangle = integralAngle;
	fixAngle = vec3{ fangle.roll,fangle.pitch,fangle.yaw };
	BTSerial.print("fixedRoll=");
	BTSerial.print(fixAngle.x);
	BTSerial.print("\t fixedPitch=");
	BTSerial.print(fixAngle.y);
	BTSerial.print("\t fixedYaw=");
	BTSerial.print(fixAngle.z);
	BTSerial.println();
	//����
	integralAngle = ZYXRPYDeg{ 0.0f,0.0f,0.0f };
	setAngle(0, 0, 0);
}

void MPU6050DMPClass::accFixStep()
{
	//У׼��Ҫ�ڵ�����ת�����½���
	if ((!isAccFixEnabled) || (millis() < lastAccFixTime + accFixPeriod)) {
		return;
	}
	else {
		if (abs(filterData[4]) > 60) return;
	}
	//������ٶ�
	filter.ReadAccGyr(filterData);
	cosG = filterData[2];
	float accX = filterData[1];
	cosG /= localG;
	accX /= localG;
	if (abs(accX * accX + cosG * cosG - 1.0f) < 0.001f) //����������1�ȣ�Ҳ����0.1%
	{
		//У׼��ֻУ׼roll��
		if(accX > 0.0f) integralAngle.roll = acos(cosG);
		else integralAngle.roll = acos(cosG) * -1.0f;
		BTSerial.print("Accfixed,roll=");
		BTSerial.println(integralAngle.roll);
		//���¶��ۼ�����ʱ
		lastAccFixTime = millis();
		startLoopMills = millis();
	}
}
void MPU6050DMPClass::integralUpdateStep()
{
	filter.getOriginData(filterData);
	float h = (micros() - nLastTime) / 1000000.0f;
	nLastTime = micros();
	integralAngle.roll += h * ((float)filterData[4] / gyr2Deg);
}
