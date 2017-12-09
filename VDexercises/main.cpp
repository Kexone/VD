#include <iostream>
#include "exercise1.h"
#include "exercise3.h"
#include "exercise4.h"
#include "exercise5.h"
#include <opencv/cv.hpp>

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
	//Exercise5().run();
	//return 0;
	bool stillRunning = true;
	while (stillRunning){ 
		int a;
		std::cout << "Select exercise:";
		std::cin >> a;
		switch(a)
		{
		case 1:
			printExercise(a);
			Exercise1().run();
			break;
		case 2:
			printExercise(a);
			Exercise1().run(1);
			break;
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
			Exercise5().run();
			break;
		case 77:
			printExercise(a);
			printExercise(1);
			Exercise1().run();
			printExercise(3);
			Exercise3().run();
			printExercise(4);
			Exercise3().run();
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

