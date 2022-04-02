#pragma once
/*后台逻辑接口*/
#include <qstring.h>
#include <string>
#include <vector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "ViewItemCar.h"
#include "ViewImage.h"
#include "ViewPath.h"
#include "qtimer.h"
#include "SerialPort.hpp"
#include "RobustMatcher.h"
#include "viewVector.h"
#include "qlabel.h"
#include "carState.h"
#include "ViewNode.h"
#include "NavigationController.h"
#include "Floyd.h"
#include "ViewCapture.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
namespace autopilot {

	/****************项目说明*****************/
	//Zorder:
	//car=2, textItem=3,point(标识点)=1.5,cachePath=1
	class Model
	{
	public:
		/*小车控制*/
		float rotateAngle = 360.0; //单位旋转角度
		float moveSpeed = 10.0; //前进后退速度
		QTimer *flushTimer;
		void carMoveForward(bool flag, QString cmd);
		void carMoveBackward(bool flag, QString cmd);
		void carTurnLeft(bool flag, QString cmd);
		void carTurnRight(bool flag, QString cmd);
		void setFlushTimer();
		void flushView();

		/*蓝牙串口*/
		bool connectBlueToothSerial();
		bool getCarSerialStatus();
		bool isCarSerialPortActivated = false;
		char incomingData[MAX_DATA_LENGTH];
		int baudRate = 9600;
		int portNum = 6;
		SerialPort* arduino = nullptr;
		std::string getPortName();
		void setPortName(int num);
		std::string listenOnce();
		/*串口信息显示*/
		long bufferSize = 1000;
		int bufferUpdateFrequency = 115200;
		/*串口命令传输和识别*/
		QString cmdStr;
		int CmdsCount = 0; //接收到的指令数量
		void readBuffer(QString str);
		void serialWrite(std::string str);
		void readArduinoFeedBack(QString str);
		bool isConnected();
		/*图像识别*/
		RobustMatcher b;
		int cameraSamplingFrequency = 10; //对视频流采样的频率
		int compressedWidth = 300; //压缩后图片宽度
		int compressedHeight = 400; //压缩后图片高度
		bool isTranslateToBW = false;//是否压缩成灰度图
		ViewVector rotateAndCompareImage(int nodeID, float carDirection); //将当前图片和固定node的图片比较
		ViewImage*  addViewImageFromNowNode(int nodeID, float rotation); //根据当前位置得到一张图片
		ViewVector SURF(float matchThreshold, cv::Mat leftImg , cv::Mat rightImg);
		void SURFMutiFiles(float matchThreshold, std::vector<std::string> leftFiles, std::vector<std::string> rightFiles);
		void SURFTest();
		/*配置文件*/
		QString settingPath;
		void readSettings(); //将设置文件中的内容读取到界面中
		void writeSettings(); //将界面的内容保存到设置文件中
		void setSettingPath(QString settingPath); //改变配置文件的路径

		/*轨迹显示*/
		QLabel* labelNavigationStatus;
		carState state; //小车当前状态
		ViewItemCar* car;
		QVector<ViewImage*> images;

		void cmdFinished(bool isStoppedByError); //接收到E指令后
		QPointF real2ScreenPos(QPointF realp); //从现实坐标转换虚拟坐标
		void flushCarViewPosition(bool isFlushPos); //根据state刷新
		void flushCarViewRotation(bool isFlushDirection);
		QVector<ViewNode*> nodes; //所有路径点的集合
		QVector<ViewPath*> paths; //所有路径的集合
		ViewPath* nowPath;
		bool isNowDrawingPath = false; //是否在绘制路径
		bool isLoadTestMapWhenStart = false; //是否在最初的时候打开测试地图

		/*自动导航*/
		NavigationController controller;
		bool getNavigationState(); //前端穿透到controller
		void sendCmd2Arduino(QString str);
		int closestNodeID = -1;//最近的节点，-1代表所有节点都在距离外
		int closestNodeIndex = -1;
		void updateClosestNodeID(); //寻找最近起点
		void addNavigationNode(); //设置导航点
		void startAutoNavigation(int pointID);
		void cancelNowPath(); //撤销当前走过的路径
		void GoodMatches(std::vector<cv::DMatch> matches,std::vector<cv::KeyPoint> keypoints1,std::vector<cv::KeyPoint> keypoints2, cv::Mat leftImg, cv::Mat rightImg);
		/*地图加载写入*/
		bool saveMapToFile(); //将当前map写入到文件中
		bool loadMap(); //从Utils给出的folder读取map


		/**
		 * 坐标说明
		 * 屏幕坐标和小车坐标是两套坐标，屏幕坐标以左上角为原点，单位长度对应一个像素
		 * 小车坐标以小车初始位置为原点，单位长度对应现实中的1cm，两个坐标的旋转的解释是一样的。
		 * 默认情况下小车现实坐标原点与画布中心重合
		**/
		void ViewInit(QWidget* window); //初始化UI界面
		QGraphicsView* pView;
		Model(QGraphicsView* view, QLabel* labelNavigationStatus,QWidget* window);
	};
}
