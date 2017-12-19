#pragma once
#include "exercises.h"
#include <random>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <random>
#include "utils.h"

class Exercise3 : public Exercises
{
public:
	void run(int i = 0) override;

private:
	float gaussian(const float A = 1.0f, const float x = 1.0f, const float y = 1.0f, const float x0 = 0.0f, const float y0 = 0.0f, const float sx = 1.5f, const float sy = 1.5f);
	float schwefel(const float x, const float y);
	std::random_device rd;
	
	const int N = 300; // number of samples
	const float x0 = -3.0; // sampling domain ranges
	const float y0 = -3.0;
	const float x1 = 3.0;
	const float y1 = 3.0;

	const int gd = 2; // dataset geometrical dimension

	const float radius = 1.f;

};
