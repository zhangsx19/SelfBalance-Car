#pragma once
#include "qgraphicsitem.h"
#include "Utils.h"
/*һ���洢�ڵ�ͼ�ϵ�ͼ��*/

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
	//ͼƬ���
	//void setImage();
	//void getImage();

	//ͼƬƥ��

	int getID();
	void setID(int _ID);
	//·����Ϣ

	QPoint getPosition();
	void setPosition(QPoint pos);

	//״̬
	void setStatus(ImageStatus sta);
	ImageStatus getStatus();

	//�����Ϣ
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
};
