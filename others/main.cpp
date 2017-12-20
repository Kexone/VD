#include <iostream>
#include "exercise3.h"
#include "exercise4.h"
#include <opencv/cv.hpp>
#include "exercise6.h"

void printExercise(int numb)
{
	std::cout << "\t****************" << std::endl;
	if (numb != 77) 
		std::cout << "\t*  EXERCISE " << numb << "  *" << std::endl;
	else
		std::cout << "\t*ALL EXERCISES *" << std::endl;
	std::cout << "\t****************" << std::endl;
}

int main()
{
	Exercise6().run();
	//return 0;
	bool stillRunning = false;
	while (stillRunning){ 
		int a;
		std::cout << "Select exercise:";
		std::cin >> a;
		switch(a)
		{
		case 3:
			printExercise(a);
			Exercise3().run();
			break;
		case 4:
			printExercise(a);
			Exercise4().run();
			break;
		case 5:
			printExercise(a);
			break;
		case 6:
			printExercise(a);
			Exercise6().run();
		case 77:
			printExercise(a);
			printExercise(3);
			Exercise3().run();
			printExercise(4);
			Exercise4().run();
			break;
		case 0:
			stillRunning = false;
			break;
		default:
			break;
		}
	}
	std::cout << " Bye..";
	return 0;
}

