#include <iostream>
#include <math.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_pXFactoring.h>
#include <string>

using namespace std;
using namespace NTL;

ZZ currentA;

//Algorithm shamelessly stolen from
//http://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/
//because i was too lazy to write this myself
//Modifications were made, though.
long printFactors(ZZ n, int max)
{
	long curMax;
	//cout << n << "\n";
	 // Print the number of 2s that divide our number
    while (n%2 == 0)
    {
       cout << "2 ";
       n = n / 2;
	   curMax = 2;
    }
	//cout << "Past first while\n";
	ZZ squareRt;
	SqrRoot(squareRt, n);
	//cout << "Past sqrt" << "\n";
	//cout << n << "\n";
	//cout << squareRt << "\n";
    // n must be odd at this point.  So we can skip one element (Note i = i +2)
    for (int i = 3; i <= max; i = i+2)
    {
        // While i divides n, print i and divide n
        while (n%i == 0)
        {
            cout << i << " ";
            n = n/i;
			curMax = i;
        }
		//cout << "Past " << i << "\n";
		SqrRoot(squareRt, n);
    }
 
    // This condition is to handle the case whien n is a prime number
    // greater than 2
    if (n > 2)
	{
		cout << n << " ";
		curMax = to_long(n);
	}
        
	cout << "\n";
	
	return curMax;
}

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

Vec< Pair< ZZ_pX, long > > checkA(ZZ_pX& base, ZZ& a, ZZ_pXModulus& m, ZZ_p& leadCoeff)
{
	//Initialize result of PowerMod
	ZZ_pX result;
	ZZ_pX monicResult;
	
	//Do modular exponentiation
	PowerMod(result, base, a, m);
	
	leadCoeff = LeadCoeff(result);
	div(monicResult, result, leadCoeff);
	
	//cout << result << "\n";
	//cout << monicResult << "\n";
	
	Vec< Pair< ZZ_pX, long > > factors;
	CanZass(factors, monicResult);  // calls "Cantor/Zassenhaus" algorithm

	return factors;
	
	
}

bool checkFactors(const Vec< Pair< ZZ_pX, long > >& factors)
{
	if (factors.length() == 0) return false;
	for (int i = 0; i < factors.length(); i++)
	{
		if (deg(factors[i].a) > 1)
			return false;
	}
	return true;
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
	
	// ZZ a;
	// a = 2L;
	// ZZ b = power(a, 410);
	// ZZ c = power(2, 400);
	// cout << "Result: " << a << " " << b << "\n";
	// cout << currentA << "\n";
	// cout << ++currentA << "\n";
	
	
	Vec< Pair< ZZ_pX, long > > factors;
	ZZ_p leadCoeff;
	while (true)
	{
		int max = 200;
		factors = checkA(base, currentA, precomp, leadCoeff);
		
		
		if (checkFactors(factors))
		{
			
			ZZ lc;
			conv(lc, leadCoeff);
			long result = printFactors(lc, max);
			if (result >= max)
			{
				currentA++;
				continue;
			}
			
			bool wasGreater = false;
			for (int i = 0; i < factors.length(); i++)
			{
				ZZ_p coeff;
				GetCoeff(coeff, factors[i].a, 0);
				conv(lc, coeff);
				if (lc >= max)
				{
					wasGreater = true;
					break;
				}
				else if (lc > result)
					result = to_long(lc);
				//printFactors(lc);
				
			}
			if (wasGreater)
			{
				currentA++;
				continue;
			}
				
			//If we made it to here, new match found!
			//Adjust max, then print out the result.
			max = result;
			cout << "--------------MATCH FOUND----------------------\n";
			cout << currentA << endl;
			cout << leadCoeff << endl;
			cout << factors << endl;
			cout << "-----------------------------------------------\n";
		}
		currentA++;
	}
	
	//Check if factors are in the right form
	cout << leadCoeff << endl;
	cout << factors << endl;
	cout << checkFactors(factors) << endl;
	cout << endl;
	ZZ lc;
	conv(lc, leadCoeff);
	printFactors(lc, 200);
	for (int i = 0; i < factors.length(); i++)
	{
		ZZ_p coeff;
		GetCoeff(coeff, factors[i].a, 0);
		conv(lc, coeff);
		printFactors(lc, 200);
	}
	
	//Short test case for checkFactors
	// ZZ pTest(2);
	// ZZ_p::init(pTest);
	// ZZ_pX toFactor;
	// toFactor = 1;
	// SetCoeff(toFactor, 2, 1);
	// Vec< Pair< ZZ_pX, long > > testFactors;
	// CanZass(testFactors, toFactor);  // calls "Cantor/Zassenhaus" algorithm
	// cout << testFactors << endl;
	// cout << checkFactors(testFactors) << endl;
	//End test case
	
	
	return 0;

}