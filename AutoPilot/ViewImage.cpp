#include "ViewImage.h"


ViewImage * ViewImage::create(int _ID, ImageStatus _status, QPoint _pos)
{
	ViewImage* vi = new ViewImage();
	vi->setID(_ID);
	vi->setStatus(_status);
	vi->setPos(_pos);
	return nullptr;
}

int ViewImage::getID()
{
	return 0;
}

void ViewImage::setID(int _ID)
{
	this->ID = _ID;
}

QPoint ViewImage::getPosition()
{
	return pos;
}

void ViewImage::setPosition(QPoint pos)
{
	this->pos = pos;
	this->setPos(pos);
}

void ViewImage::setStatus(ImageStatus sta)
{
	this->status = sta;
	switch (sta)
	{
	case OutOfRange:
		this->setPixmap(QPixmap(path + "photoPosGrey.png"));
		break;
	case Detected:
		this->setPixmap(QPixmap(path + "photoPosBlue.png"));
		break;
	case ErrorPos:
		this->setPixmap(QPixmap(path + "photoPosRed.png"));
		break;
	default:
		break;
	}
}

ImageStatus ViewImage::getStatus()
{
	return ImageStatus();
}

void ViewImage::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	//œ‘ æÕº∆¨

}
