#include "MPU6050DMP.h"
//#include "C:\Users\xuan\Documents\Arduino\libraries\SoftwareSerial\src\SoftwareSerial.h"
//SoftwareSerial BTSerial(8, 9);
void setup()
{
	/* GyroSetup() ��ʼ����ִ��У׼
	_calibrationTime: ������У׼�Ĳ���ʱ��(ms),Խ��У׼Ч��Խ��(�Ƽ�15000-35000)
	_periodLoopTimes: ������ִ�еĺ���������ʾ���ݣ�����gyroLoopÿִ�ж��ٴ�ʱִ��һ��
	_localG: �����������ٶȣ���С��ͨ��GyroTestSampling�õ����ݵ���matlab����ƽ���õ�
	_nCalibTimes: �����������ʱÿ�ֲ���������Խ��Խ����Ч��Խ��
	_FS_SEL: �����ǵ����������ٶȵ�λ���ã���MPU6050��dataSheet
	_accFix: ������̶�һ��ʱ�����У׼����ˮƽ���õĵ���״̬��roll�Ǻ�pitch��У׼��0(�ᵼ�²���Ƶ�ʽ���)
	_display������������Ƕ���Ϣ
	*/
	Serial.begin(115200);
	/*BTSerial.begin(115200);*/
	/*MPU6050DMP.GyroSetup(15000,80,15899,300,1,true,true);*/
}

void loop()
{
	//���ú�����MPU5060DMP.h

	//MPU6050DMP.GyroLoopStart(); //����ʹGyroLoop��һ�ε��ý�����������ú���
	while (1) {
		//MPU6050DMP.GyroLoop(); //ÿ�ε��ö����ݽ���һ�β������������ȸ�Ƶ�ص���
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
