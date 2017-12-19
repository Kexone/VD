#ifndef GNUPLOT_H
#define GNUPLOT_H
#include <iostream>
#include <vector>
#include "../gnuplot_i.hpp"

class GnuPlot
{
	
private:
	Gnuplot g;

public:
	GnuPlot()
	{
		Gnuplot g("title");
	}
	void setCmds(std::vector< std::string > commands )
	{
		for(auto &command : commands)
		{
			g.cmd(command);
		}
	}

	void savePlot(std::string name)
	{
		g.savetops(name);
	}

	void showPlot()
	{
		g.showonscreen();
	}
};
#endif //GNUPLOT_H