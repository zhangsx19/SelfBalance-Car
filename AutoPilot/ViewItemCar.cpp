#include "ViewItemCar.h"
#include "Utils.h"
void ViewItemCar::init(QPoint pos)
{
	this->setPos(pos);
	this->setPixmap(QPixmap(Utils::getUIFolder() + "carPos.png"));
}
