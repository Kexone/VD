#include "exercise1.h"
#include <iostream>
#include <map>
#include <vector>
#include <cmath>

#include "../gnuplot_i.hpp"



#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)
#include <conio.h>   //for getch(), needed in wait_for_key()
#include <windows.h> //for Sleep()
void sleep(int i) { Sleep(i * 1000); }
#endif


#define SLEEP_LGTH 2  // sleep time in seconds
#define NPOINTS    50 // length of array

void wait_for_key(); // Programm halts until keypress

void Exercise1::run()
{
	// if path-variable for gnuplot is not set, do it with:
	 Gnuplot::set_GNUPlotPath("D:/Programs/gnuplot/bin");
	// set a special standard terminal for showonscreen (normally not needed),
	//   e.g. Mac users who want to use x11 instead of aqua terminal:
	// Gnuplot::set_terminal_std("x11");

	std::cout << "*** example of gnuplot control through C++ ***" << std::endl << std::endl;

	//
	// Using the GnuplotException class
	//
	try
	{
		Gnuplot g1("lines");

		//
		// Slopes
		//
		std::cout << "*** plotting slopes" << std::endl;
		g1.set_title("Slopes\\nNew Line");

		std::cout << "y = x" << std::endl;
		g1.plot_slope(1.0, 0.0, "y=x");

		std::cout << "y = 2*x" << std::endl;
		g1.plot_slope(2.0, 0.0, "y=2x");

		std::cout << "y = -x" << std::endl;
		g1.plot_slope(-1.0, 0.0, "y=-x");
		g1.unset_title();

		//
		// Equations
		//
		g1.reset_plot();
		std::cout << std::endl << std::endl << "*** various equations" << std::endl;

		std::cout << "y = sin(x)" << std::endl;
		g1.plot_equation("sin(x)", "sine");

		std::cout << "y = log(x)" << std::endl;
		g1.plot_equation("log(x)", "logarithm");

		std::cout << "y = sin(x) * cos(2*x)" << std::endl;
		g1.plot_equation("sin(x)*cos(2*x)", "sine product");

		//
		// Styles
		//
		g1.reset_plot();
		std::cout << std::endl << std::endl << "*** showing styles" << std::endl;

		std::cout << "sine in points" << std::endl;
		g1.set_pointsize(0.8).set_style("points");
		g1.plot_equation("sin(x)", "points");

		std::cout << "sine in impulses" << std::endl;
		g1.set_style("impulses");
		g1.plot_equation("sin(x)", "impulses");

		std::cout << "sine in steps" << std::endl;
		g1.set_style("steps");
		g1.plot_equation("sin(x)", "steps");

		//
		// Save to ps
		//
		g1.reset_all();
		std::cout << std::endl << std::endl << "*** save to ps " << std::endl;

		std::cout << "y = sin(x) saved to test_output.ps in working directory" << std::endl;
		g1.savetops("test_output");
		g1.set_style("lines").set_samples(300).set_xrange(0, 5);
		g1.plot_equation("sin(12*x)*exp(-x)").plot_equation("exp(-x)");

		g1.showonscreen(); // window output


						   //
						   // User defined 1d, 2d and 3d point sets
						   //
		std::vector<double> x, y, y2, dy, z;

		for (int i = 0; i < NPOINTS; i++)  // fill double arrays x, y, z
		{
			x.push_back((double)i);             // x[i] = i
			y.push_back((double)i * (double)i); // y[i] = i^2
			z.push_back(x[i] * y[i]);           // z[i] = x[i]*y[i] = i^3
			dy.push_back((double)i * (double)i / (double)10); // dy[i] = i^2 / 10
		}
		y2.push_back(0.00); y2.push_back(0.78); y2.push_back(0.97); y2.push_back(0.43);
		y2.push_back(-0.44); y2.push_back(-0.98); y2.push_back(-0.77); y2.push_back(0.02);


		g1.reset_all();
		std::cout << std::endl << std::endl << "*** user-defined lists of doubles" << std::endl;
		g1.set_style("impulses").plot_x(y, "user-defined doubles");

		g1.reset_plot();
		std::cout << std::endl << std::endl << "*** user-defined lists of points (x,y)" << std::endl;
		g1.set_grid();
		g1.set_style("points").plot_xy(x, y, "user-defined points 2d");

		g1.reset_plot();
		std::cout << std::endl << std::endl << "*** user-defined lists of points (x,y,z)" << std::endl;
		g1.unset_grid();
		g1.plot_xyz(x, y, z, "user-defined points 3d");

		g1.reset_plot();
		std::cout << std::endl << std::endl << "*** user-defined lists of points (x,y,dy)" << std::endl;
		g1.plot_xy_err(x, y, dy, "user-defined points 2d with errorbars");


		//
		// Multiple output screens
		//
		std::cout << std::endl << std::endl;
		std::cout << "*** multiple output windows" << std::endl;

		g1.reset_plot();
		g1.set_style("lines");
		std::cout << "window 1: sin(x)" << std::endl;
		g1.set_grid().set_samples(600).set_xrange(0, 300);
		g1.plot_equation("sin(x)+sin(x*1.1)");

		g1.set_xautoscale().replot();



		wait_for_key();

	}
	catch (GnuplotException ge)
	{
		std::cout << ge.what() << std::endl;
	}


	std::cout << std::endl << "*** end of gnuplot example" << std::endl;


}
void wait_for_key()
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__)  // every keypress registered, also arrow keys
	std::cout << std::endl << "Press any key to continue..." << std::endl;

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	_getch();
#elif defined(unix) || defined(__unix) || defined(__unix__) || defined(__APPLE__)
	cout << endl << "Press ENTER to continue..." << endl;

	std::cin.clear();
	std::cin.ignore(std::cin.rdbuf()->in_avail());
	std::cin.get();
#endif
	return;
}
