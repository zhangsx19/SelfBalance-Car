#pragma once
/*��̨�߼��ӿ�*/
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

	/****************��Ŀ˵��*****************/
	//Zorder:
	//car=2, textItem=3,point(��ʶ��)=1.5,cachePath=1
	class Model
	{
	public:
		/*С������*/
		float rotateAngle = 360.0; //��λ��ת�Ƕ�
		float moveSpeed = 10.0; //ǰ�������ٶ�
		QTimer *flushTimer;
		void carMoveForward(bool flag, QString cmd);
		void carMoveBackward(bool flag, QString cmd);
		void carTurnLeft(bool flag, QString cmd);
		void carTurnRight(bool flag, QString cmd);
		void setFlushTimer();
		void flushView();

		/*��������*/
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
		/*������Ϣ��ʾ*/
		long bufferSize = 1000;
		int bufferUpdateFrequency = 115200;
		/*����������ʶ��*/
		QString cmdStr;
		int CmdsCount = 0; //���յ���ָ������
		void readBuffer(QString str);
		void serialWrite(std::string str);
		void readArduinoFeedBack(QString str);
		bool isConnected();
		/*ͼ��ʶ��*/
		RobustMatcher b;
		int cameraSamplingFrequency = 10; //����Ƶ��������Ƶ��
		int compressedWidth = 300; //ѹ����ͼƬ���
		int compressedHeight = 400; //ѹ����ͼƬ�߶�
		bool isTranslateToBW = false;//�Ƿ�ѹ���ɻҶ�ͼ
		ViewVector rotateAndCompareImage(int nodeID, float carDirection); //����ǰͼƬ�͹̶�node��ͼƬ�Ƚ�
		ViewImage*  addViewImageFromNowNode(int nodeID, float rotation); //���ݵ�ǰλ�õõ�һ��ͼƬ
		ViewVector SURF(float matchThreshold, cv::Mat leftImg , cv::Mat rightImg);
		void SURFMutiFiles(float matchThreshold, std::vector<std::string> leftFiles, std::vector<std::string> rightFiles);
		void SURFTest();
		/*�����ļ�*/
		QString settingPath;
		void readSettings(); //�������ļ��е����ݶ�ȡ��������
		void writeSettings(); //����������ݱ��浽�����ļ���
		void setSettingPath(QString settingPath); //�ı������ļ���·��

		/*�켣��ʾ*/
		QLabel* labelNavigationStatus;
		carState state; //С����ǰ״̬
		ViewItemCar* car;
		QVector<ViewImage*> images;

		void cmdFinished(bool isStoppedByError); //���յ�Eָ���
		QPointF real2ScreenPos(QPointF realp); //����ʵ����ת����������
		void flushCarViewPosition(bool isFlushPos); //����stateˢ��
		void flushCarViewRotation(bool isFlushDirection);
		QVector<ViewNode*> nodes; //����·����ļ���
		QVector<ViewPath*> paths; //����·���ļ���
		ViewPath* nowPath;
		bool isNowDrawingPath = false; //�Ƿ��ڻ���·��
		bool isLoadTestMapWhenStart = false; //�Ƿ��������ʱ��򿪲��Ե�ͼ

		/*�Զ�����*/
		NavigationController controller;
		bool getNavigationState(); //ǰ�˴�͸��controller
		void sendCmd2Arduino(QString str);
		int closestNodeID = -1;//����Ľڵ㣬-1�������нڵ㶼�ھ�����
		int closestNodeIndex = -1;
		void updateClosestNodeID(); //Ѱ��������
		void addNavigationNode(); //���õ�����
		void startAutoNavigation(int pointID);
		void cancelNowPath(); //������ǰ�߹���·��
		void GoodMatches(std::vector<cv::DMatch> matches,std::vector<cv::KeyPoint> keypoints1,std::vector<cv::KeyPoint> keypoints2, cv::Mat leftImg, cv::Mat rightImg);
		/*��ͼ����д��*/
		bool saveMapToFile(); //����ǰmapд�뵽�ļ���
		bool loadMap(); //��Utils������folder��ȡmap


		/**
		 * ����˵��
		 * ��Ļ�����С���������������꣬��Ļ���������Ͻ�Ϊԭ�㣬��λ���ȶ�Ӧһ������
		 * С��������С����ʼλ��Ϊԭ�㣬��λ���ȶ�Ӧ��ʵ�е�1cm�������������ת�Ľ�����һ���ġ�
		 * Ĭ�������С����ʵ����ԭ���뻭�������غ�
		**/
		void ViewInit(QWidget* window); //��ʼ��UI����
		QGraphicsView* pView;
		Model(QGraphicsView* view, QLabel* labelNavigationStatus,QWidget* window);
	};
}
