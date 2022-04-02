#include "ViewPath.h"
#include "Utils.h"
#include "qmath.h"
ViewPathStep::ViewPathStep(ViewPathStep* lastStep, ViewPoint _direction)
{
	this->last = lastStep;
	this->next = nullptr;
	this->setPixmap(QPixmap(Utils::getUIFolder() + "pathLine.png"));
	this->setVisible(false);
	this->vec = _direction;
	this->length = 0;
}

float ViewPathStep::getLength()
{
	return length;
}

void ViewPathStep::flush()
{
	//刷新显示的外观
	if (Utils::floatEqual(vec.getLength(), 0) == false) {
		//缩放
		this->setScale((qreal)getLength());
		this->setRotation(vec.getRotationDeg());
		this->setVisible(true);
	}
}


float ViewPathStep::getRotationDeg(ViewPathStep * lastStep, ViewPathStep * nextStep)
{
	return lastStep->vec.getRotationDeg() - nextStep->vec.getRotationDeg();
}

void ViewPath::addStep()
{
	//当前的偏角是上一步的角度
	ViewPathStep* step = new ViewPathStep(nowStep,nowStep->vec);
	nowStep->next = step;
	nowStep = step;
	flush();
}

void ViewPath::addRotation(float rotationDeg)
{
	ViewPoint vec1 = nowStep->vec;
	//求出vec1对应的角度

	float theta = vec1.getRotationDeg();
	//添加上新增角度
	theta -= rotationDeg; //顺时针是正，于是顺时针是减
	while (theta > 360.0f || theta < 0) {
		if (theta > 360.0f) theta -= 360.0f;
		if (theta < 0) theta += 360.0f;
	}
	float length = nowStep->vec.getLength();
	vec1.x = cosf(theta * M_PI / 180.0f);
	vec1.y = sinf(theta * M_PI / 180.0f);
	nowStep->vec.x = vec1.x * length;
	nowStep->vec.y = vec1.y * length;
	flush();
}

void ViewPath::addLength(float length)
{
	float k = nowStep->vec.getLength();
	k = (k + length) / k;
	nowStep->vec.x *= k;
	nowStep->vec.y *= k;
	flush();
}

void ViewPath::pathEnd()
{
	nowStep->next = nullptr;
	pEnd = nowStep->vec;
	pStart = startStep->vec;
}

void ViewPath::flush()
{
	nowStep->flush();
}



ViewPath::ViewPath(ViewPoint start, ViewPoint direction)
{
	pStart = start;
	if (Utils::floatEqual(direction.x * direction.x + direction.y * direction.y, 1.0f) == false) {
		Utils::log(true, "ViewPath: illegal direction vector.");
	}
	startStep = new ViewPathStep(nullptr,direction);
	nowStep = startStep;
}

float ViewPoint::getLength()
{
	return sqrt(x*x + y * y);
}

float ViewPoint::getRotationDeg()
{
	if (Utils::floatEqual(getLength(), 0) == true) {
		Utils::log(true, "ViewPoint: illegal direction.");
		return 0.0f;
	}
	float theta = acos(x / this->getLength());
	if (y > 0) {
		theta *= 180.0f / M_PI;
	}
	else {
		theta = 2 * M_PI - theta;
		theta *= 180.0f / M_PI;
	}
	return theta;
}
