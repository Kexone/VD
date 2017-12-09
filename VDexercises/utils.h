#pragma once
#include <conio.h>   
#include <windows.h> 
class Utils
{
public:
 template <typename T>
 static T sqr(T x) { return x * x; }
 void static wait_for_key()
 {
	 std::cout << std::endl << "Press any key to continue..." << std::endl;
	 FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	 _getch();
	 return;
 }

};