#include "exercise1.h"
#include <vector>
#include "gnuPlot.h"

#include "utils.h"


void Exercise1::run(int i)
{
	std::vector < std::string > commands;
	GnuPlot gp;
	if (i == 0) {
		commands = {
			"set enhanced color font 'sans, 6' fontscale 1.0 linewidth 1 rounded background 'white' size 10cm,6cm ",
			"set encoding utf8",
			"set datafile separator ','",
			"set xdata time",
			"set timefmt '%Y.%m.%d'",
			"set format x '%m.%Y'",
			"set style line 80 linetype 1 linecolor '#808080' linewidth 2 ",
			"set border 3 back linestyle 80",
			"set style line 81 lt 0",
			"set style line 81 lt rgb '0x00D3D3D3'",
			"set style line 81 lw 0.5",
			"set grid back linestyle 81",
			"set xtics nomirror",
			"set ytics nomirror",
			"set title 'Temperature Measurement Over Several Days' font 'sans-Bold'",
			"set xlabel 'Time [day]'",
			"set ylabel 'Avg. temp. [°C]'",
			"set yrange [-30:50]",
			"set xrange ['1975.1.1':'2016.9.1']",
			"set style line 1 lt rgb '#A00000' lw 2 pt 1",
			"plot 'dataset.csv' using 1:(($2  - 32) * 5.0/9.0) title 'Temps' with lines"
		};
	}
	else
	{
		FILE* file;
		std::cout << "som tu";
		Py_Initialize();
		file = fopen("./data/cv2.py", "r");
		PyRun_SimpleFile(file, "cv2.py");
		Py_Finalize();
	}
	gp.setCmds(commands);
	gp.showPlot();
	Utils::wait_for_key();

}

