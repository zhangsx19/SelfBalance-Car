
#include "MPU6050Filter.h"
#include "Wire.h"
SoftwareSerial BTSerial(blueToothTX, blueToothRX);

void MPU6050FilterClass::init(int FS_SEL)
{
	Wire.begin();
	Wire.beginTransmission(MPU);
	Wire.write(0x6B);
	Wire.write(0);
	Wire.endTransmission(true);
	int sel = 0;
	if (FS_SEL == 0) { sel = 0x00; gyr2Deg = 131.0f; }
	if (FS_SEL == 1) { sel = 0x8; gyr2Deg = 65.5f; }
	if (FS_SEL == 2) { sel = 0x10; gyr2Deg = 32.8f; }
	if (FS_SEL == 3) { sel = 0x18; gyr2Deg = 16.4f; }
	Wire.beginTransmission(MPU);
	Wire.write(0x1B);
	Wire.write(sel);
	Wire.endTransmission(true);
}

void MPU6050FilterClass::ReadAccGyrUnFixed(bool isAddMode, int *pVals) {
	Wire.beginTransmission(MPU);
	Wire.write(0x3B);
	Wire.requestFrom(MPU, nValCnt * 2, true);
	Wire.endTransmission(true);
	if (isAddMode) {
		for (int i = 0; i < nValCnt; ++i) {
			pVals[i] += (Wire.read() << 8 | Wire.read());
		}
	}
	else {
		for (int i = 0; i < nValCnt; ++i) {
			pVals[i] = Wire.read() << 8 | Wire.read();
		}
	}
}
//��MPU6050�������ٶȼ������������¶Ⱥ��������ٶȼ�
void MPU6050FilterClass::ReadAccGyr(int *pVals) {
	Wire.beginTransmission(MPU);
	Wire.write(0x3B);
	Wire.requestFrom(MPU, nValCnt * 2, true);
	Wire.endTransmission(true);
	for (int i = 0; i < nValCnt; ++i) {
		pVals[i] = ((Wire.read() << 8 | Wire.read()) - calibData[i]);
	}
}
void MPU6050FilterClass::getOriginData(int * nVal)
{
	ReadAccGyr(nVal);
}

//�Դ�����������ͳ�ƣ�У׼ƽ��ƫ����
void MPU6050FilterClass::Calibration()
{
	float valSums[7] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0 };
	BTSerial.println("Running Calibration: first part. Don't move MPU6050 sensor.");
	bool isFixed = false;
	int calRound = 1;
	while (isFixed == false) {
		BTSerial.print("Calibration round start...Round=");
		BTSerial.println(calRound);
		//���
		for (int k = 0; k < 7; k++) {
			valSums[k] = 0.0f;
		}
		for (int i = 0; i < nCalibTimes; ++i) {
			int mpuVals[7];
			ReadAccGyrUnFixed(false, mpuVals);
			for (int j = 0; j < nValCnt; ++j) {
				valSums[j] += mpuVals[j];
			}
		}
		//��Ȩ���
		isFixed = true;
		for (int i = 0; i < nValCnt; ++i) {
			int data = valSums[i] / (float)nCalibTimes;
			if (abs(calibData[i] - data) >= 4 && i != 3) { //thresHold=4 ��ֵС��4ʱ��������������4ʱ��Ư̫ǿ��
				if (calRound == 1) {
					calibData[i] = data;
				}
				else {
					calibData[i] = 0.5f * (float)calibData[i] + 0.5f * (float)data;
				}
				BTSerial.print("Fixing...[");
				BTSerial.print(i);
				BTSerial.print("]=");
				BTSerial.println(data);
				isFixed = false;
			}
		}
		calRound++;
	}
	BTSerial.println("Calibration done!");
	calibData[2] -= standardG; //��оƬZ����ֱ���£��趨��̬�����㡣
}

MPU6050FilterClass filter;