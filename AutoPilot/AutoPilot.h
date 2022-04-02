#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_AutoPilot.h"
#include "Model.h"
#include "qtimer.h"
class AutoPilot : public QMainWindow
{
    Q_OBJECT

public:
	AutoPilot(QWidget *parent = Q_NULLPTR);
	autopilot::Model* model;
	//设置文件
	void updateSettingsUI(); //从model加载设定值
	void updateModelFromSettingsUI(); //从UI导入设定值到model
	void updateAutoPilot();
	void updateCarControl();
	void updateCV();
	void updateBlueToothSerial();
	void updateCarBlueTooth();
	//小车运动
	bool flagForward = false;
	bool flagBackward = false;
	bool flagLeft = false;
	bool flagRight = false;

	void carMoveForward();
	void carMoveBackward();
	void carTurnLeft();
	void carTurnRight();
	//蓝牙串口
	void serialTextUpdate();
private:
	Ui::MainWindow ui;
	QString mapFolderPath;
	QString testFolderPath;
	QString cacheFolderPath;
	QString settingsFilePath;
	void chooseMapFolder();
	void chooseTestFolder();
	void chooseCacheFolder();
	void chooseSettingsFile();
	void connectCarBluetoothSerial();
	QTimer* bufferUpdateTimer = nullptr;
};
