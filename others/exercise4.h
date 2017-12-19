#pragma once
#include "exercises.h"
#include <opencv/cv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
class Exercise4 : public Exercises
{
public:
	void run(int i = 0) override;
private:
	cv::Point2f calcRungeKutta(cv::Mat flowN, cv::Point2f p, float dt);
	void calcCurl(cv::Mat & flow, cv::Mat & curl);
	cv::Point2f getSubPixel(const cv::Mat &flow, cv::Point2f pt);
	void vizualizeData(cv::Mat &flowNorm, cv::Mat & matLines);
	void normalizeFlow(cv::Mat &flow, cv::Mat& flowN, float& magMax);
	void loadData();

	cv::Point2f _point;
	std::vector<cv::Point2f> _points;
	std::vector<cv::Mat> flows;
	double scale = 5.3;

};
