#include "exercises.h"
#include <iostream>
#include "exercise1.h"
#include "exercise3.h"


int main()
{
	bool stillRunning = true;
	while (stillRunning){ 
		int a;
		std::cout << "Select exercise:";
		std::cin >> a;
		switch(a)
		{
		case 1:
			Exercise1().run();
			break;
		case 2:
			//Exercise2().run();
			break;
		case 3:
			Exercise3().run();
			break;
		case 4:
			//Exercise4().run();
			break;
		case 0:
			stillRunning = false;
			break;
		}
	}
	return 0;
}
