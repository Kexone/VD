#pragma once
#include "exercises.h"
#include <opencv2/core/matx.hpp>
#include <opencv2/core/types.hpp>
#include "syntheticDepth.h"
#include <opencv2/core/persistence.hpp>
#include <iomanip>
#include <sstream>
#include <opencv/cv.hpp>
class Exercise6 : public Exercises
{
public:
	void run(int i = 0) override;
private:

	std::vector<cv::Mat> depths;
	SyntheticDepth sd;


	void loadDataDepthMaps();
	void loadSyntheticDepth();
	void calcNormals(cv::Mat &depth, cv::Mat &normals, cv::Mat &quant, int patchSize);
	cv::Vec3f getXYZ_vector(int u, int v, cv::Size halfSize, float pixV, float f = 570.342f);
};
