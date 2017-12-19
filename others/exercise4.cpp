#include "exercise4.h"

void Exercise4::run(int i)
{
	cv::Mat flowN;
	loadData();
	cv::Mat matLines = cv::Mat::zeros(flows[0].rows * scale, flows[0].cols * scale, CV_8UC3);
	_point = cv::Point2f(55,52);
	for (int i = 0; i < flows.size(); i++)
	{
		vizualizeData(flows[i], matLines);
		cv::waitKey(1);
	}
	cv::waitKey(0);
}

cv::Point2f Exercise4::calcRungeKutta(cv::Mat flow, cv::Point2f p, float dt)
{
	cv::Point2f K1 = getSubPixel(flow, p			 ) * dt ;
	cv::Point2f K2 = getSubPixel(flow, p + K1 * 0.5f ) * dt;
	cv::Point2f K3 = getSubPixel(flow, p + K2 * 0.5f ) * dt;
	cv::Point2f K4 = getSubPixel(flow, p + K3		 ) * dt;
	
	return p + ((1.f / 6.f) * (K1 + 2 * K2 + 2 * K3 + K4));
}

cv::Point2f Exercise4::getSubPixel(const cv::Mat &flow, cv::Point2f pt) {
	cv::Mat patch;
	cv::remap(flow, patch, cv::Mat(1, 1, CV_32FC2, &pt), cv::noArray(),
		cv::INTER_LINEAR, cv::BORDER_REFLECT_101);
	cv::Vec2f v = patch.at<cv::Vec2f>(0, 0);

	return cv::Point2f(v[0], v[1]);
}

void Exercise4::calcCurl(cv::Mat& flow, cv::Mat& curl)
{
	for(int y = 1; y < flow.rows - 1; y++)
	{
		for (int x = 1; x < flow.cols - 1; x++)
		{
			cv::Point2f y0 = flow.at<cv::Point2f>(y - 1, x);
			cv::Point2f yn = flow.at<cv::Point2f>(y + 1, x);
			cv::Point2f x0 = flow.at<cv::Point2f>(y, x - 1);
			cv::Point2f xn = flow.at<cv::Point2f>(y, x + 1);

			curl.at<float>(x, y) =  x0.x - xn.x - y0.y - yn.y;
		}
	}
}

void Exercise4::loadData()
{
	std::ostringstream ss;
	cv::FileStorage fs;
	std::cout << "Loading files.." ;
	for (int i = 0, n = 1; i < 200; i++, n++)
	{
		ss.str("");
		ss << "data/flow/u" << std::setw(5) << std::setfill('0') << i << ".yml";
		cv::Mat fl;
		fs.open(ss.str().c_str(), cv::FileStorage::Mode::READ);
		fs["flow"] >> fl;
		flows.push_back(fl);
		fl.release();
		printf("\rLoading files..%0.1f %%", (100.0 * n) / 200);
	}
}

void Exercise4::normalizeFlow(cv::Mat &flow, cv::Mat& flowN, float& magMax)
{
	flowN = cv::Mat(flow.size(), CV_32FC4);

	for (int y = 0; y < flow.rows; y++)
	{
		for (int x = 0; x < flow.cols; x++)
		{
			cv::Vec2f vec = flow.at<cv::Vec2f>(y, x);
			float magnitude = std::sqrt((vec[0] * vec[0]) + (vec[1] * vec[1]));
			if (magnitude > magMax)	magMax = magnitude;
			float rot = 0;
			if( y > 0 && x > 0 && y < flow.rows - 1 && x < flow.cols)
			{
				cv::Vec2f y0 = flow.at<cv::Vec2f>(y - 1, x);
				cv::Vec2f yn = flow.at<cv::Vec2f>(y + 1, x);
				cv::Vec2f x0 = flow.at<cv::Vec2f>(y, x - 1);
				cv::Vec2f xn = flow.at<cv::Vec2f>(y, x + 1);

				rot = xn[1] - x0[1] - y0[0] - yn[0];
			}
				if (magnitude != 0) flowN.at<cv::Vec4f>(y, x) = cv::Vec4f(vec[0] / magnitude, vec[1] / magnitude, magnitude, rot);
				else flowN.at<cv::Vec4f>(y, x) = cv::Vec4f(0, 0, magnitude, rot);
			
		}
	}
}


void Exercise4::vizualizeData(cv::Mat &flowNorm, cv::Mat & matLines)
{
	cv::Mat scene, curl;
	curl = cv::Mat::zeros(flowNorm.rows, flowNorm.cols, CV_32FC1);

	//cv::Mat directions[4];
	//cv::split(flowNorm, directions);
	calcCurl(flowNorm, curl);
	cv::normalize(curl, scene, 0, 255, CV_MINMAX);
	scene.convertTo(scene, CV_8UC1);
	cv::applyColorMap(scene, scene, cv::COLORMAP_HSV);
	cv::resize(scene, scene, cv::Size(), scale, scale, CV_INTER_CUBIC);
	double min, max;
	cv::minMaxLoc(flowNorm,& min,& max);
	auto arrowScale = static_cast<float>(MAX(abs(min), abs(max)) / scale);
	for (int y = 0; y < flowNorm.rows; y++) {
		for (int x = 0; x < flowNorm.cols; x++) {
			cv::Point2f vec = flowNorm.at<cv::Point2f>(y, x) * arrowScale;
			cv::Point2f center((x) * scale, (y ) * scale);
			cv::Point2f begin = center - vec;
			cv::Point2f end = center + vec;

			cv::arrowedLine(scene, begin, end, cv::Scalar::all(0), 1, 4, 0, 0.2);
		}
	}
	for(int i = 0; i < 50; i++) {
		cv::Point2f mPoint = calcRungeKutta(flowNorm, _point,0.1);

		cv::line(matLines, _point * scale, mPoint * scale, cv::Scalar(255,255,250), 2, 4);
	
		_point = mPoint;
	}
	cv::circle(scene, _point * scale, 4, cv::Scalar(0, 0, 255), -1, CV_AA);
	cv::add(scene, matLines, scene);
	cv::imshow("Flow", scene);
}
