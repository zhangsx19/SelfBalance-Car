#pragma once
#include "qgraphicsitem.h"

struct ViewPoint {
public:

	float x = 0.0f;
	float y = 0.0f;
	float getLength();
	float getRotationDeg(); //��ȡ��ת�Ƕ�
};
/*һС��·��*/
class ViewPathStep : public QGraphicsPixmapItem
{

public:
	ViewPathStep* last, *next; //����ṹ
	ViewPoint vec; //��λ����, �������С������
	float length; //��С�����ƶ�����
	ViewPathStep(ViewPathStep* lastStep, ViewPoint _direction);
	float getLength();
	void flush();
	//����
	//�������֮���ת�����Deg���涨˳ʱ��תΪ������ʱ��Ϊ��
	static float getRotationDeg(ViewPathStep* lastStep, ViewPathStep* nextStep);
};
class ViewPath
{

public:
	ViewPoint pStart, pEnd; //��βλ��
	ViewPathStep* startStep;
	ViewPathStep* nowStep;
	void addStep(); //���һ��
	void addRotation(float rotationDeg); //�����ǰ������
	void addLength(float length); //�����������
	void pathEnd(); //����·��
	void flush();
	ViewPath(ViewPoint start, ViewPoint direction);
};

