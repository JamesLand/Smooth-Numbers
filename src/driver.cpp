#include <iostream>
#include <math.h>
#include <NTL/ZZ.h>

using namespace NTL;

int currentA = 0;

int getNextA()
{
	return 0;
}

int main()
{
	ZZ a(2L);
	ZZ b = power(a, 400);
	//ZZ c = power(2, 400);
	std::cout << "Result: " << a << " " << b << "\n";
	return 0;

}