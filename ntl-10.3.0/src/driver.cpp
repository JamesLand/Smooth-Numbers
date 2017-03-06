#include <iostream>
#include <math.h>
#include <NTL/ZZ.h>
#include <sstream>
#include <string>
#include <stdio.h>


using namespace std;
using namespace NTL;

ZZ currentA;

ZZ getFirstA()
{
	string str("112885193112928372112889036");
	for (int i = 0; i < 95; i++)
		str = str+'0';
	ZZ A(NTL::INIT_VAL, str.c_str());
	return A;
}
/*
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
}*/

int main()
{
	ZZ currentA = getFirstA();
	
	ZZ max = power2_ZZ(410);
	ZZ min = power2_ZZ(400);
	bool withinRange = ((currentA > min) && (currentA < max));
	
	cout << currentA << "\n";
	printf("Within range? %s\n", withinRange ? "true" : "false");
	cout << ++currentA << "\n";
	
	return 0;

}