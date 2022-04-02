#pragma once
#include "qgraphicsitem.h"
#include "Utils.h"
/*一幅存储在地图上的图像*/

enum ImageStatus { OutOfRange, Detected, ErrorPos };
class ViewImage : public QGraphicsPixmapItem
{

private:
	QString path = Utils::getUIFolder();
	int ID;
	QPoint pos;
	ImageStatus status;
public:

	static ViewImage* create(int _ID, ImageStatus _status,QPoint _pos);
	//图片相关
	//void setImage();
	//void getImage();

	//图片匹配

	int getID();
	void setID(int _ID);
	//路径信息

	QPoint getPosition();
	void setPosition(QPoint pos);

	//状态
	void setStatus(ImageStatus sta);
	ImageStatus getStatus();

	//点击信息
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
};
