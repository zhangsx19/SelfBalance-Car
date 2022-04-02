#pragma once
#include "qgraphicsitem.h"

struct ViewPoint {
public:

	float x = 0.0f;
	float y = 0.0f;
	float getLength();
	float getRotationDeg(); //获取旋转角度
};
/*一小段路径*/
class ViewPathStep : public QGraphicsPixmapItem
{

public:
	ViewPathStep* last, *next; //链表结构
	ViewPoint vec; //单位向量, 方向代表小车方向
	float length; //大小代表移动距离
	ViewPathStep(ViewPathStep* lastStep, ViewPoint _direction);
	float getLength();
	void flush();
	//工具
	//检测两步之间的转向度数Deg，规定顺时针转为正，逆时针为负
	static float getRotationDeg(ViewPathStep* lastStep, ViewPathStep* nextStep);
};
class ViewPath
{

public:
	ViewPoint pStart, pEnd; //首尾位置
	ViewPathStep* startStep;
	ViewPathStep* nowStep;
	void addStep(); //添加一步
	void addRotation(float rotationDeg); //添加向前的数据
	void addLength(float length); //添加向后的数据
	void pathEnd(); //结束路径
	void flush();
	ViewPath(ViewPoint start, ViewPoint direction);
};

