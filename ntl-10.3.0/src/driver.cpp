#include <iostream>
#include <math.h>
#include <NTL/ZZ.h>

using namespace std;
using namespace NTL;

ZZ currentA;

ZZ getFirstA()
{
	ZZ a, b, c;
	a = power(ZZ(10), 95);
	b = power(ZZ(10), 104);
	c = power(ZZ(10), 113);
	a = a * 112889036L;
	b = b * 112928372L;
	c = c * 112885193L;
	
	ZZ firstA;
	firstA = a + b + c;
	
	//currentA
	return firstA;
}

int main()
{
	
	
	currentA = getFirstA();
	
	ZZ a;
	a = 2L;
	ZZ b = power(a, 410);
	//ZZ c = power(2, 400);
	cout << "Result: " << a << " " << b << "\n";
	cout << currentA << "\n";
	cout << ++currentA << "\n";
	return 0;

}