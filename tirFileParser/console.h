#pragma once
#include <Windows.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum Console_Color
{
	Black,
	Dark_Blue,
	Green,
	Blue_Grey,
	Brown,
	Purple,
	Khaki,
	Light_Grey,
	Grey,
	Light_Blue,
	Light_Green,
	Cyan,
	Red,
	Pink,
	Yellow,
	White
};