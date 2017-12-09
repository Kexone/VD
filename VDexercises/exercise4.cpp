#include "exercise4.h"
#include <opencv/cv.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/persistence.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>

std::vector<cv::Mat> flows;
void vizualizeData(cv::Mat &flowNorm, float &magMax);
void normalizeFlow(cv::Mat &flow, cv::Mat &flowN, float &magMax);
void loadData()
{
	std::ostringstream ss;
	cv::FileStorage fs;
	for (int i = 0; i < 200; i++)
	{
		ss.str("");
		ss << "data/flow/u" << std::setw(5) << std::setfill('0') << i << ".yml";
		cv::Mat fl;
		fs.open(ss.str().c_str(), cv::FileStorage::Mode::READ);
		fs["flow"] >> fl;
		flows.push_back(fl);
		fl.release();
	}
}
void Exercise4::run(int i)
{
	cv::Mat flowN;
	loadData();
	for (int i = 0; i < flows.size(); i++)
	{
		float magMax = 0;
		normalizeFlow(flows[i], flowN,magMax);
		vizualizeData(flowN, magMax);
		flowN.release();
	}

	cv::waitKey(0);
}


void vizualizeData(cv::Mat &flowNorm, float &magMax)
{
	cv::Mat scene;
	int scale = 5;
	cv::Mat directions[4];
	cv::split(flowNorm, directions);
	cv::normalize(directions[3], directions[3], 0.0f, 1.0f, CV_MINMAX);
	directions[3].convertTo(scene, CV_8UC1, 255.0f);
	cv::applyColorMap(scene, scene, cv::COLORMAP_BONE);
	cv::resize(scene, scene, cv::Size(), scale, scale, CV_INTER_CUBIC);

	for (int y = 0; y < flowNorm.rows; y++)
	{
		for(int x = 0; x < flowNorm.cols; x++)
		{
			cv::Vec4f vec = flowNorm.at<cv::Vec4f>(y, x);
			float magN = vec[2] / magMax + 0.1f;
			float offsetX = (vec[0] > 0) ? 0 : scale;
			float offsetY = (vec[1] > 0) ? 0 : scale;

			cv::Point2f center(x * scale + offsetX, y * scale + offsetY);
			cv::Point2f dir(center.x + vec[0] * scale * magN, center.y + vec[1] * scale * magN);

			if (magN < 0.2f) magN = 0.2f;
			cv::arrowedLine(scene, center, dir, cv::Scalar(1), 1, 8, 0, static_cast<double> (magN / 1.8f));
		}
	}
	cv::imshow("Flow", scene);
	cv::waitKey(15);
}

void normalizeFlow(cv::Mat &flow, cv::Mat& flowN, float& magMax)
{
	flowN = cv::Mat(flow.size(), CV_32FC4);

	for (int y = 0; y < flow.rows; y++)
	{
		for (int x = 0; x < flow.cols; x++)
		{
			cv::Vec2f vec = flow.at<cv::Vec2f>(y, x);
			float magnitude = std::sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]));

			if (magMax < magnitude) 
				magMax = magnitude;

			float rot = 0;
			if (y > 0 && y < flow.rows - 1 && x > 0 && x < flow.cols - 1)
			{
				cv::Vec2f x1 = flow.at<cv::Vec2f>(y, x - 1);
				cv::Vec2f x2 = flow.at<cv::Vec2f>(y, x + 1);
				cv::Vec2f y1 = flow.at<cv::Vec2f>(y - 1, x);
				cv::Vec2f y2 = flow.at<cv::Vec2f>(y + 1, x);

				rot = (x2[1] - x1[1]) - (y1[0] - y2[0]);   // dx - dy
			}
				if (magnitude != 0)	flowN.at<cv::Vec4f>(y, x) = cv::Vec4f(vec[0] / magnitude, vec[1] / magnitude, magnitude, rot);
				else flowN.at<cv::Vec4f>(y, x) = cv::Vec4f(0, 0, magnitude, rot);
			
		}
	}
}
