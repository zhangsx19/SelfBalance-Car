#include "MPU6050DMP.h"
//#include "C:\Users\xuan\Documents\Arduino\libraries\SoftwareSerial\src\SoftwareSerial.h"
//SoftwareSerial BTSerial(8, 9);
void setup()
{
	/* GyroSetup() 初始化并执行校准
	_calibrationTime: 陀螺仪校准的采样时间(ms),越长校准效果越好(推荐15000-35000)
	_periodLoopTimes: 周期性执行的函数（如显示数据），在gyroLoop每执行多少次时执行一次
	_localG: 当地重力加速度，大小可通过GyroTestSampling得到数据导入matlab中求平均得到
	_nCalibTimes: 修正固有误差时每轮采样次数，越大越慢，效果越好
	_FS_SEL: 陀螺仪的最大测量角速度档位设置，见MPU6050的dataSheet
	_accFix: 开启后固定一段时间进行校准，在水平放置的低速状态将roll角和pitch角校准到0(会导致采样频率降低)
	_display：开启后输出角度信息
	*/
	Serial.begin(115200);
	/*BTSerial.begin(115200);*/
	/*MPU6050DMP.GyroSetup(15000,80,15899,300,1,true,true);*/
}

void loop()
{
	//常用函数见MPU5060DMP.h

	//MPU6050DMP.GyroLoopStart(); //尽量使GyroLoop第一次调用紧跟在这个调用后面
	while (1) {
		//MPU6050DMP.GyroLoop(); //每次调用对数据进行一次采样，尽量均匀高频地调用
		/*char test[5] = "F0100	";
		Serial.println(test);*/
		char message[5];
		if (Serial.available() > 0)
		{
			for (int i = 0; i < 5; i++)
			{
				message[i] = (char)Serial.read();

			}
			Serial.println(message/* (char)Serial.read()*/);
		}
		delay(1000);
	}
}
