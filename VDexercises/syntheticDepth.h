#pragma once
#include <opencv2/core/mat.hpp>


struct SyntheticDepth
{
public:
	int cameraWidth;
	int cameraHeight;
	double fovY;
	double pixelSize;
	cv::Mat viewFrom;
	cv::Mat viewAt;
	cv::Mat viewMatrix;
	cv::Mat viewMatrixT;
	cv::Mat data;
	SyntheticDepth() {}
	SyntheticDepth(int cw, int ch,double fY, double pS, cv::Mat vF, cv::Mat vA, cv::Mat vM, cv::Mat vMT, cv::Mat d) : 
	cameraWidth(cw), cameraHeight(ch),fovY(fY),pixelSize(pS), viewFrom(vF),
		viewAt(vA), viewMatrix(vM), viewMatrixT(vMT), data(d) {}
private:

};
