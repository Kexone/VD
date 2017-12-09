#include "exercise6.h"
#include "syntheticDepth.h"
#include <opencv2/core/persistence.hpp>
#include <iomanip>
#include <sstream>
#include <opencv/cv.hpp>

std::vector<cv::Mat> depths;
SyntheticDepth sd;


void computeDepthMap(cv::Mat depthM);
void loadSyntheticDepth()
{
	std::ostringstream ss;
	cv::FileStorage fs;
	fs.open("data/synthetic_depth/scene_eda_2.yml", cv::FileStorage::Mode::READ);
	fs["camera_width"] >> sd.cameraWidth;
	fs["camera_height"] >> sd.cameraHeight;
	fs["fov_y"] >> sd.fovY;
	fs["pixel_size"] >> sd.pixelSize;
	fs["view_from"] >> sd.viewFrom;
	fs["view_at"] >> sd.viewAt;
	fs["view_matrix"] >> sd.viewMatrix;
	fs["view_t_matrix"] >> sd.viewMatrixT;
	fs["depth"] >> sd.data;

	fs.release();
}

void loadDataDepthMaps()
{
	std::ostringstream ss;
	cv::FileStorage fs;
	for (int i = 1; i < 152; i++)
	{
		ss.str("");
		ss << "data/depth/dept.yml (" << i << ").gz";
		cv::Mat dp;
		fs.open(ss.str().c_str(), cv::FileStorage::Mode::READ);
		fs["depth"] >> dp;
		depths.push_back(dp);
		dp.release();
	}
	fs.release();
}

void Exercise6::run(int i)
{
	loadDataDepthMaps();
	loadSyntheticDepth();
	cv::imshow("sdepth", sd.data);
	for (int i = 0; i < depths.size(); i++)
	{
		computeDepthMap(depths[i]);
		cv::waitKey(15);
	}
	cv::waitKey(0);	
}

void computeDepthMap(cv::Mat depthM)
{
	int patch = 0;
	
	cv::Mat norml(depthM.size(), CV_32FC3);

	float Ex = 0, Ey = 0, Ex2 = 0, Ey2 = 0;

	for(int x = 0; x < norml.rows; x++ )
	{
		for (int y = 0; y < norml.cols; y++)
		{
			float depth = depthM.at<float>(x, y);
			cv::Vec3f xyz;// = xyz(x, norml.rows / 2, y, norml.cols / 2, depth);
		}
	}
}