#include "AutoPilot.h"
#include "Model.h"
#include "qtimer.h"
#include "qpropertyanimation.h"
#include <iostream>
#include "qfiledialog.h"
using namespace std;

AutoPilot::AutoPilot(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//初始化前端的逻辑连接
	connect(ui.ButtonFlushUI, &QPushButton::clicked , this,&AutoPilot::updateSettingsUI);
	connect(ui.ButtonFlushSettings, &QPushButton::clicked, this, &AutoPilot::updateModelFromSettingsUI);
	connect(ui.ButtonAutoPilotUpdate, &QPushButton::clicked, this, &AutoPilot::updateAutoPilot);
	connect(ui.ButtonCarControlUpDate, &QPushButton::clicked, this, &AutoPilot::updateCarControl);
	connect(ui.ButtonCVUpdate, &QPushButton::clicked, this, &AutoPilot::updateCV);
	connect(ui.ButtonAutoPilotUpdate, &QPushButton::clicked, this, &AutoPilot::updateBlueToothSerial);
	connect(ui.ButtonCarBlueToothConnect, &QPushButton::clicked, this, &AutoPilot::connectCarBluetoothSerial);
	connect(ui.ButtonConnectCar, &QPushButton::clicked, this, &AutoPilot::connectCarBluetoothSerial);

	connect(ui.ButtonChooseCacheFolder, &QPushButton::clicked, this, &AutoPilot::chooseCacheFolder);
	connect(ui.ButtonChooseDataPath, &QPushButton::clicked, this, &AutoPilot::chooseTestFolder);
	connect(ui.ButtonChooseMapFolder, &QPushButton::clicked, this, &AutoPilot::chooseMapFolder);
	connect(ui.ButtonChooseSettingsPath, &QPushButton::clicked, this, &AutoPilot::chooseSettingsFile);
	//初始化模型
	model = new autopilot::Model(ui.ViewNavigation, this);
	//初始化小车控制模块的连接
	connect(ui.ButtonForward, &QPushButton::pressed, this, &AutoPilot::carMoveForward);
	connect(ui.ButtonForward, &QPushButton::released, this, &AutoPilot::carMoveForward);
	
}

void AutoPilot::updateSettingsUI()
{
	cout << "INFO: updateSettingsUI()" << endl;
	//更新设置界面所有的UI
	ui.EditCOM->setText(QString::number(model->portNum, 10));
	ui.EditBaudRate->setText(QString::number(model->baudRate, 10));
	ui.RadioConvertToBW->setChecked(model->isTranslateToBW);
	ui.EditBufferSize->setText(QString::number(model->bufferSize, 10));
	ui.EditBufferUpdateFrequency->setText(QString::number(model->bufferUpdateFrequency, 10));
	ui.EditCameraSamplingFrequency->setText(QString::number(model->cameraSamplingFrequency, 10));
	ui.EditCompressedWidth->setText(QString::number(model->compressedWidth,10));
	ui.EditCompressedHeight->setText(QString::number(model->compressedHeight, 10));
	ui.EditMoveSpeed->setText(QString(to_string(model->moveSpeed).c_str()));
	ui.EditRotateAngle->setText(QString(to_string(model->rotateAngle).c_str()));
	ui.RadioLoadTestMapOnStart->setChecked(model->isLoadTestMapWhenStart);
	//更新路径
	mapFolderPath = model->mapFolderPath;
	testFolderPath = model->testFolder;
	cacheFolderPath = model->cacheFolder;
	settingsFilePath = model->settingPath;
}

void AutoPilot::updateModelFromSettingsUI()
{
	//更新setting位置
	model->setSettingPath(settingsFilePath);
	//各个模块全部更新
	updateAutoPilot();
	updateBlueToothSerial();
	updateCarBlueTooth();
	updateCarControl();
	updateCV();
	//写入json
	model->writeSettings();
}

void AutoPilot::updateAutoPilot()
{
	model->mapFolderPath = mapFolderPath;
	model->isLoadTestMapWhenStart = ui.RadioLoadTestMapOnStart->isChecked();
}

void AutoPilot::updateCarControl()
{
	model->rotateAngle = ui.EditRotateAngle->text().toFloat();
	model->moveSpeed = ui.EditMoveSpeed->text().toFloat();
}

void AutoPilot::updateCV()
{
	model->cacheFolder = cacheFolderPath;
	model->testFolder = testFolderPath;
	model->cameraSamplingFrequency = ui.EditCameraSamplingFrequency->text().toInt();
	model->compressedWidth = ui.EditCompressedWidth->text().toInt();
	model->compressedHeight = ui.EditCompressedHeight->text().toInt();
	model->isTranslateToBW = ui.RadioConvertToBW->isChecked();
}

void AutoPilot::updateBlueToothSerial()
{
	model->bufferSize = ui.EditBufferSize->text().toInt();
	model->bufferUpdateFrequency = ui.EditBufferUpdateFrequency->text().toInt();
}

void AutoPilot::updateCarBlueTooth()
{
	model->portNum = ui.EditCOM->text().toInt();
	model->baudRate = ui.EditBaudRate->text().toInt();
}

void AutoPilot::carMoveForward()
{
	flagForward = !flagForward;

	model->carMoveForward(flagForward);

}

void AutoPilot::carMoveBackward()
{
	flagBackward = !flagBackward;

	model->carMoveBackward(flagBackward);
}

void AutoPilot::carTurnLeft()
{
	flagLeft = !flagLeft;

	model->carMoveBackward(flagLeft);
}

void AutoPilot::carTurnRight()
{
	flagRight = !flagRight;

	model->carMoveBackward(flagRight);
}



void AutoPilot::serialTextUpdate()
{
	QString str = QString::fromStdString(model->listenOnce());
	if(str.isEmpty() == false) ui.BrowserBuffer->append(str);
}

void AutoPilot::chooseMapFolder()
{
	mapFolderPath = QFileDialog::getExistingDirectory(
		this, 
		tr("Choose Map Folder"),
		"/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	cout << "INFO: choosed directory:" << mapFolderPath.data() << endl;
}

void AutoPilot::chooseTestFolder()
{
	testFolderPath = QFileDialog::getExistingDirectory(
		this,
		tr("Choose Test Folder"),
		"/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	cout << "INFO: choosed directory:" << testFolderPath.data() << endl;
}

void AutoPilot::chooseCacheFolder()
{
	cacheFolderPath = QFileDialog::getExistingDirectory(
		this,
		tr("Choose Cache Folder"),
		"/home",
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	cout << "INFO: choosed directory:" << cacheFolderPath.data() << endl;
}

void AutoPilot::chooseSettingsFile()
{
	settingsFilePath = QFileDialog::getOpenFileUrl(
		this,
		tr("Choose settings.json"),
		QUrl("/home"),
		"Settings (*.json)").toString();
	cout << "INFO: choosed file:" << settingsFilePath.data() << endl;
}

void AutoPilot::connectCarBluetoothSerial()
{
	//尝试打开端口
	updateCarBlueTooth();
	if (model->connectBlueToothSerial() == true) {
		if (bufferUpdateTimer != nullptr) delete bufferUpdateTimer;
		bufferUpdateTimer = new QTimer(this);
		//bufferUpdateTimer->setInterval(1000 / model->bufferUpdateFrequency);
		bufferUpdateTimer->setInterval(100);
		bufferUpdateTimer->start();
		connect(bufferUpdateTimer, &QTimer::timeout, this, &AutoPilot::serialTextUpdate);
	}
}

