#include "AutoPilot.h"
#include <QtWidgets/QApplication>
#include "RobustMatcher.h"
#include "Model.h"
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include "qdebug.h"
#include "Utils.h"
using namespace cv::xfeatures2d;
using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
    /*QApplication a(argc, argv);
    AutoPilot w;
    w.show();
    return a.exec();*/
	/*autopilot::Model a;
	a.SURFTest();*/
	/*Mat srcImage1 = imread("1.jpg", 1);
	Mat srcImage2 = imread("2.jpg", 1);*/
	RobustMatcher b;
	string leftfile = "C:\\Users\\xuan\\Downloads\\AutoPilot-master\\Data\\test\\7.jpg";
	string rightfile = "C:\\Users\\xuan\\Downloads\\AutoPilot-master\\Data\\test\\6.jpg";
	std::vector<cv::DMatch> matches;
	std::vector<cv::KeyPoint> keypoints1;
	std::vector<cv::KeyPoint> keypoints2;
	b.match(leftfile, rightfile, matches, keypoints1, keypoints2);
	waitKey(0);
	return 0;
}
//测试robustMatch,因为我们Utils里面文件夹路径不同所以没有写成test函数(我一加test//就报错，所以直接写了完整路径）