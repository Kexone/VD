#include "exercise3.h"
#include <fstream>

float Exercise3::gaussian(const float A, const float x, const float y, const float x0, const float y0, const float sx, const float sy)
{
	return A * static_cast<float>(std::exp(-(
		(Utils::sqr<float>(x - x0) / (2 * (Utils::sqr<float>(sx)))) +
		(Utils::sqr<float>(y - y0) / (2 * (Utils::sqr<float>(sy))))
		)));
}

float Exercise3::schwefel(const float x, const float y)
{
	float tmp = (-x * sin(sqrt(abs(x))));
	tmp += (-y * sin(sqrt(abs(y))));
	return tmp;
}

void Exercise3::run(int i)
{
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distX(x0, x1);
	std::uniform_real_distribution<float> distY(y0, y1);
	cv::Mat sample_points = cv::Mat(N, gd, CV_32F);
	cv::Mat sample_values = cv::Mat(N, 1, CV_32F);
	// TODO: fill in sample points and sample values, i.e. {p_i, f_i}
	for (int i = 0; i < N; i++)
	{
		float x = distX(gen);
		float y = distY(gen);
		sample_points.at<float>(i, 0) = x ;
		sample_points.at<float>(i, 1) = y ;
		//sample_values.at<float>(i) = 1000.0f * schwefel(x, y) ;
		sample_values.at<float>(i) = gaussian(1.0f, x, y);
	}

	// k-NN structures
	cv::flann::KDTreeIndexParams index = cv::flann::KDTreeIndexParams();
	cv::flann::Index nn = cv::flann::Index(sample_points, index);
	cv::flann::SearchParams search_params = cv::flann::SearchParams(256);

	cv::Mat gauss(480, 640, CV_32FC3);
	cv::Mat nearnest(480, 640, CV_32FC3);
	
	// reconstruction
	cv::Mat query = cv::Mat(1, gd, CV_32F);
	cv::Mat indices = cv::Mat(N, 1, CV_32S);
	cv::Mat dists = cv::Mat(N, 1, CV_32F);

//	std::ofstream fs;
	//fs.open("gauss.txt");
	// for each element of reconstructed domain do
	for (int y = 0; y < gauss.rows; y++) {
		for (int x = 0; x < gauss.cols; x++) {
			float xx = ((x / static_cast<float>(gauss.cols)) *(x1 - x0) + x0);
			float yy = ((y / static_cast<float>(gauss.rows)) * (y1 - y0) + y0);
			query.at<float>(0) = xx;
			query.at<float>(1) = yy;

			//nn.knnSearch(query, indices, dists, 10, search_params);
			const int no_indices = nn.radiusSearch(query, indices, dists, radius, N, search_params);
			float weight = 0, weightV = 0;

			for (int i = 0; i < no_indices; ++i)
			{
				float value = sample_values.at<float>(indices.at<int>(i));
				float distance = dists.at<float>(i);
				//float actualWeight = SQR(MAX(0, distance) / (distance));
				//float actualWeight = SQR(MAX(0, radius - distance) / (radius * distance));
				float actualWeight = SQR(MAX(0, radius - distance) / (radius * distance));
				weightV += value * actualWeight;
				weight += actualWeight;
			}
			// store result of interpolation 
			nearnest.at<cv::Vec3f>(y,x) = cv::Vec3f(0.f, weightV / weight,0.f);
			float color = weightV / weight;
			gauss.at<cv::Vec3f>(y, x) = cv::Vec3f(0, sample_values.at<float>(indices.at<int>(0)),0.0f);
		}
	}
	//fs.close();
	cv::imshow("Gauss", gauss);
	cv::imshow("Nearnest neighbors", nearnest);
	cv::waitKey(0);
}
