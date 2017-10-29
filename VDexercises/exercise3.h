#pragma once
#include "exercises.h"

class Exercise3 : public Exercises
{
public:
	void run() override;

private:
	const int N = 300; // number of samples
	const float x0 = -3.0f; // sampling domain range
	const float y0 = -3.0f;
	const float x1 = 3.0f;
	const float y1 = 3.0f;
	const float radius = 3.0f;

	const int gd = 2; // dataset geometrical dimension
};
