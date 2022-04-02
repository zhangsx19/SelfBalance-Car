#include "MPU6050DMP.h"

MPU6050DMPClass MPU6050DMP;

void MPU6050DMPClass::periodicAction()
{
	if (nowRound < targetRound) {
		nowRound++;
		//这里放每轮都需要的设置
		accFixStep();
		return;
	}
	else {
		nowRound = 0;
		//自定义代码
		//display();
	}
	//这行要放在最后
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

//指定间隔输出调试信息
void MPU6050DMPClass::display() {
	//这次放要显示的文字
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
	filter.init(3); //检测每秒最大角度：3=2000,2=1000,1=500,0=250
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
	periodicAction(); //周期性执行的函数
	delay(sampleDelay); //延迟
}
void MPU6050DMPClass::GyroLoopStart(bool _isAccFixEnabled = true)
{
	//开启加速度修正，并重置所有计时器
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
	//检查零漂
	startLoopMills = millis();
	nLastTime = micros();
	fixAngle = vec3{ 0,0,0 };
	long endms = millis() + (long)secondFixTime;
	while (millis() < endms) {
		GyroLoop();
	}
	//记录角
	ZYXRPYDeg fangle = integralAngle;
	fixAngle = vec3{ fangle.roll,fangle.pitch,fangle.yaw };
	BTSerial.print("fixedRoll=");
	BTSerial.print(fixAngle.x);
	BTSerial.print("\t fixedPitch=");
	BTSerial.print(fixAngle.y);
	BTSerial.print("\t fixedYaw=");
	BTSerial.print(fixAngle.z);
	BTSerial.println();
	//重置
	integralAngle = ZYXRPYDeg{ 0.0f,0.0f,0.0f };
	setAngle(0, 0, 0);
}

void MPU6050DMPClass::accFixStep()
{
	//校准需要在低速旋转条件下进行
	if ((!isAccFixEnabled) || (millis() < lastAccFixTime + accFixPeriod)) {
		return;
	}
	else {
		if (abs(filterData[4]) > 60) return;
	}
	//计算加速度
	filter.ReadAccGyr(filterData);
	cosG = filterData[2];
	float accX = filterData[1];
	cosG /= localG;
	accX /= localG;
	if (abs(accX * accX + cosG * cosG - 1.0f) < 0.001f) //限制最大误差1度，也就是0.1%
	{
		//校准，只校准roll角
		if(accX > 0.0f) integralAngle.roll = acos(cosG);
		else integralAngle.roll = acos(cosG) * -1.0f;
		BTSerial.print("Accfixed,roll=");
		BTSerial.println(integralAngle.roll);
		//重新对累计误差计时
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
