#include "exercise4.h"
#include <opencv2/videoio.hpp>
#include <opencv2/opencv.hpp>
#include <sstream>
#include <iomanip>
#include <vector>

std::vector<cv::Mat> flows;
void vizualizeData(cv::Mat &scene, cv::Mat &flow);
void loadData()
{
	std::ostringstream ss;
	cv::FileStorage fs;
	for (int i = 0; i < 200; i++)
	{
		ss.str("");
		ss << "data/flow/u" << std::setw(5) << std::setfill('0') << i << ".yml";
		cv::Mat fl;
		fs.open(ss.str(), cv::FileStorage::READ);
		fs["flow"] >> fl;
		flows.emplace_back(fl);
		fl.release();
	}
}
void Exercise4::run()
{
	cv::Mat scene;
	loadData();
	for (int i = 0; i < flows.size(); i++)
	{
		vizualizeData(scene, flows[i]);
	//	cv::imshow("Flow", scene);
		cv::waitKey(5);
	}

	cv::waitKey(0);
}


void vizualizeData(cv::Mat &scene, cv::Mat &flow)
{
	// Init variables
	int scale = 7;
	float magMax = 0;
	cv::Mat flowNorm(flow.size(), CV_32FC4);

}