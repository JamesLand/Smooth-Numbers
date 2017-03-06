#include <iostream>
#include <math.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_pXFactoring.h>
#include <string>

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

void checkA(ZZ_pX& base, ZZ& a, ZZ_pXModulus& m)
{
	//Initialize result of PowerMod
	ZZ_pX result;
	ZZ_pX monicResult;
	
	//Do modular exponentiation
	PowerMod(result, base, a, m);
	
	ZZ_p leadCoeff;
	leadCoeff = LeadCoeff(result);
	div(monicResult, result, leadCoeff);
	
	cout << result << "\n";
	cout << monicResult << "\n";
	
	Vec< Pair< ZZ_pX, long > > factors;
	CanZass(factors, monicResult);  // calls "Cantor/Zassenhaus" algorithm

	cout << factors << "\n";
}

int main()
{
	
	//p = 2;
	string pStr("837583943092107483758343358937591");
	ZZ p(INIT_VAL, pStr.c_str()); 
	ZZ_p::init(p);
	cout << p << "\n";
	
	// Create the base polynomial x^5 + 2
	ZZ_pX base;
	base = 2;
	SetCoeff(base, 5, 1); 
	cout << base << endl;
	
	// Create the mod polynomial x^6 + x - 44
	ZZ_pX mod;
	mod = -44;
	SetCoeff(mod, 1, 1);
	SetCoeff(mod, 6, 1);
	cout << mod << endl;
	
	// Precomputing for mod polynomial
	ZZ_pXModulus precomp;
	build(precomp, mod);
	
	currentA = getFirstA();
	
	ZZ a;
	a = 2L;
	ZZ b = power(a, 410);
	//ZZ c = power(2, 400);
	cout << "Result: " << a << " " << b << "\n";
	cout << currentA << "\n";
	cout << ++currentA << "\n";
	
	
	
	checkA(base, currentA, precomp);
	return 0;

}