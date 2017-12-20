#include "exercise6.h"




void Exercise6::loadSyntheticDepth()
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

void Exercise6::loadDataDepthMaps()
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
	cv::Mat normals, quantized;
	loadDataDepthMaps();
	//cv::imshow("sdepth", sd.data);
	for (int i = 0; i < depths.size(); i++)
	{
		calcNormals(depths[i], normals,quantized,3);
		cv::imshow("Normals", depths[i]);
		cv::waitKey(15);
	}

	loadSyntheticDepth();
	calcNormals(sd.data, normals, quantized, 3);
	cv::imshow("Normals", normals);
	cv::waitKey(0);	
}

void Exercise6::calcNormals(cv::Mat &depth, cv::Mat &normals, cv::Mat &quant, int patchSize)
{
	int patch = 0;
	int offset = patchSize / 2;

	normals = cv::Mat::zeros(depth.size(), CV_32FC3);
	quant = cv::Mat::zeros(depth.size(), CV_8UC1);
	cv::Size halfSize((int)depth.cols / 2, (int)depth.rows / 2);

	for(uint y = 0; y < depth.rows - offset; y++)
	{
		for( uint x = 0; x < depth.cols - offset; x++)
		{
			float Ex = 0, Ey = 0, Ez = 0;
			int nValid = patchSize * patchSize;

			for( uint yy = y - offset; yy < y+offset+1; yy++)
			{
				for (uint xx = x - offset; xx < x+offset+1; xx++)
				{
					float pix = depth.at<float>(yy, xx);

					if( pix <= 0 )
					{
						nValid--;
						continue;
					}

					cv::Vec3f vec = getXYZ_vector(xx, yy, halfSize, pix);

					Ex += vec[0];
					Ey += vec[1];
					Ez += vec[2];
				}
			}

			if (nValid <= 0) continue;

			float normPix = 1.f / nValid;
			Ex *= normPix;
			Ey *= normPix;
			Ez *= normPix;

			cv::Mat covarMat = cv::Mat::zeros(3, 3, CV_32FC1);
			for(uint yy = y - offset; yy < y+offset+1; yy++)
			{
				for(uint xx = x - offset; xx < x+offset-1;xx++)
				{
					float pix = depth.at<float>(yy, xx);
					if (pix <= 0) continue;

					cv::Vec3f vec = getXYZ_vector(xx, yy, halfSize, pix);

					covarMat.at<float>(0, 0) += (vec[0] - Ex) * (vec[0] - Ex);
					covarMat.at<float>(0, 1) += (vec[0] - Ex) * (vec[0] - Ex);
					covarMat.at<float>(0, 2) += (vec[0] - Ex) * (vec[0] - Ex);

					covarMat.at<float>(1, 0) += (vec[1] - Ey) * (vec[1] - Ey);
					covarMat.at<float>(1, 1) += (vec[1] - Ey) * (vec[1] - Ey);
					covarMat.at<float>(1, 2) += (vec[1] - Ey) * (vec[1] - Ey);
					
					covarMat.at<float>(1, 0) += (vec[2] - Ez) * (vec[2] - Ez);
					covarMat.at<float>(1, 1) += (vec[2] - Ez) * (vec[2] - Ez);
					covarMat.at<float>(1, 2) += (vec[2] - Ez) * (vec[2] - Ez);
				}
			}
			covarMat *= normPix;
			cv::Mat eigenVal, eigenVec;
			cv::eigen(covarMat, eigenVal, eigenVec);
			cv::Vec3f normal = eigenVec.row(2);

			normals.at<cv::Vec3f>(y, x) = normal;
		}
	}
}

cv::Vec3f Exercise6::getXYZ_vector(int u, int v, cv::Size halfSize, float pixV, float f)
{
	return cv::Vec3f(((u - halfSize.width) * pixV) / f, -((-v + halfSize.height) * pixV) / f, pixV);
}
