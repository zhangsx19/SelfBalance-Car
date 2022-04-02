#include "Utils.h"
#include <iostream>
QString Utils::data("F:\\Project\\2020SummerHardwareDesign\\APGUI\\AutoPilot\\Data\\");
QString Utils::getDataFolder()
{
	return data;
}

QString Utils::getUIFolder()
{
	return data + "UI\\";
}

QString Utils::getMapFolder()
{
	return data + "map\\";
}

QString Utils::getSettingsFolder()
{
	return data + "settings\\";
}

bool Utils::floatEqual(float left, float right, float prec)
{
	if (abs(left - right) < prec) return true;
	else return false;
}

void Utils::log(bool isError, std::string str)
{
	if (isError) {
		std::cout << "[ERROR]:" << str << std::endl;

	}
	else {
		std::cout  << str << std::endl;
	}
}
