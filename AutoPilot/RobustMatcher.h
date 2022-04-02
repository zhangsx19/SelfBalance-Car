#pragma once
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <ctime>
#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include <iostream>
#include <string>
#include <opencv2/calib3d/calib3d.hpp>
#include "viewVector.h"

class RobustMatcher {
private:

	// pointer to the feature point detector object
	cv::Ptr<cv::FeatureDetector> detector;
	// pointer to the feature descriptor extractor object
	cv::Ptr<cv::DescriptorExtractor> extractor;
	cv::Ptr<cv::DescriptorMatcher> matcher;
	float ratio; // max ratio between 1st and 2nd NN
	bool refineF; // if true will refine the F matrix
	double distance; // min distance to epipolar
	double confidence; // confidence level (probability)
	bool match_judge = false;
public:
	//distance，ratio越大精度越小,confidence相反
	RobustMatcher();

	int ratioTest(std::vector<std::vector<cv::DMatch>>& matches);

	// Insert symmetrical matches in symMatches vector
	void symmetryTest(const std::vector<std::vector<cv::DMatch>>& matches1,
		const std::vector<std::vector<cv::DMatch>>& matches2,
		std::vector<cv::DMatch>& symMatches);

	// Identify good matches using RANSAC
	// Return fundemental matrix
	cv::Mat ransacTest(const std::vector<cv::DMatch>& matches,
		const std::vector<cv::KeyPoint>& keypoints1,
		const std::vector<cv::KeyPoint>& keypoints2,
		std::vector<cv::DMatch>& outMatches);

	autopilot::ViewVector match(cv::Mat leftImg, cv::Mat rightImg, // input images
		std::vector<cv::DMatch>& matches, // output matches and keypoints
		std::vector<cv::KeyPoint>& keypoints1, std::vector<cv::KeyPoint>& keypoints2);
};