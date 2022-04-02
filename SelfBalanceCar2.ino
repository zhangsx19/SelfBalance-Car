// Visual Micro is in vMicro>General>Tutorial Mode
//
/*
	Name:       moduleTest.ino
	Created:	2020/7/18 19:44:17
	Author:     DESKTOP-493ILV9\admin
*/

// Define User Types below here or use a .h file
//

#include <MsTimer2.h>
#include <Servo.h>
#define TOUCHONCE while(1){if(digitalRead(pinSensor) == HIGH) break;}delay(500)
#include <U8glib.h>
#include <SoftwareSerial.h>
#include "PID.h"

//�������
const int pinDIR1 = 7; //�ı䷽��
const int pinDIR2 = 8;
const int pinSTP1 = 10;//��timer1���ƣ�����
const int pinSTP2 = 3; //��timer2���ƣ��ҵ��
const int pinSLP1 = 5;//LOW��ʾ������ߣ�����״̬��Ҫ�ֶ����������Խ��ͷ��ȣ�
const int pinSLP2 = 4;
//�ⲿ����
const int pinSensor = 13; //����������

/*OLED��ʾ��*/
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_FAST);
/*��������*/
extern SoftwareSerial BTSerial;//����pin�Ŷ����MPU6050Filter.h

//һ���Ի�ͼ����
void draw(char* str, int line) {
	if (line < 1) return;

	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr(0, 10 * line, str);
	} while (u8g.nextPage());
}

//pwm����
void timerSetup() {
	cli(); //ֹͣ�ж�
	//set timer1 interrupt
	TCNT1 = 0;//initialize counter value to 0
	TCCR1A = _BV(COM1B1) | _BV(WGM11) | _BV(WGM10);
	TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS12);
	// set compare match register for 1hz increments
	OCR1A = 70;//����Ƶ��
	OCR1B = 1; //ռ�ձ�b/a��ֻ��pin10�����pin9��pwm������

	//set timer2 interrupt
	TCNT2 = 0;
	TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20); //Set Timer2 to varying top limit fast PWM mode
	TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS21);

	OCR2A = 70; //����Ƶ��
	OCR2B = 1; //ռ�ձ�b/a��ֻ��pin3�����, pin11��pwm������

	sei();//allow interrupts
}
// The setup() function runs once each time the micro-controller starts
void setup()
{
	//�����������ת״̬
	pinMode(pinSLP1, OUTPUT);
	pinMode(pinSLP2, OUTPUT);
	pinMode(pinSTP1, OUTPUT);
	pinMode(pinSTP2, OUTPUT);
	pinMode(pinDIR1, OUTPUT);
	pinMode(pinDIR2, OUTPUT);
	pinMode(pinSensor, INPUT);
	digitalWrite(pinSLP1, LOW);
	digitalWrite(pinSLP2, LOW);
}

// Add the main program code into the continuous loop() function
void loop()
{
	draw("Setup...", 3);
	delay(2000);
	draw("BlueTooth:9600", 1);
	BTSerial.begin(9600); //������
	TOUCHONCE;
	draw("GyroSetup", 1);
	MPU6050DMP.GyroSetup(0);
	draw("PIDSetup", 1);
	PID.PIDsetup(pinDIR1, pinDIR2, pinSTP1, pinSTP2, pinSLP1, pinSLP2);
	draw("TimerSetup", 1);
	timerSetup();
	draw("Ready :)", 1);
	delay(1000);
	draw("Touch to loop", 1);
	TOUCHONCE;
	draw("Looping", 1);
	MPU6050DMP.GyroLoopStart(false);
	PID.PIDLoopStart();
	while (1)
	{
		MPU6050DMP.GyroLoop();
		PID.PIDLoop();
		if (digitalRead(pinSensor) == HIGH) break;
	}
	digitalWrite(pinSLP1, LOW);
	digitalWrite(pinSLP2, LOW);
	draw("Loop end", 1);
	delay(500); //������������
	TOUCHONCE;
}