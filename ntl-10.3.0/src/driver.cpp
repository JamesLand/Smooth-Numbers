#include <iostream>
#include <math.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_pXFactoring.h>
#include <string>

using namespace std;
using namespace NTL;

ZZ currentA;

int maxSmoothness = 1000;

int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43,	47,	
53,	59,	61,	67, 71,	73,	79,	83,	89,	97,	101, 103, 107, 109, 113, 127, 131,
137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223,
227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009};

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
       //cout << "2 ";
       n = n / 2;
	   curMax = 2;
    }
	//cout << "Past first while\n";
	//ZZ squareRt;
	//SqrRoot(squareRt, n);
	//cout << "Past sqrt" << "\n";
	//cout << n << "\n";
	//cout << squareRt << "\n";
    // n must be odd at this point.  So we can skip one element (Note i = i +2)
    for (int i = 0; primes[i] <= max; i = i+1)
    {
		int p = primes[i];
        // While i divides n, print i and divide n
        while (n%p == 0)
        {
            //cout << p << " ";
            n = n/p;
			curMax = p;
        }
		//cout << "Past " << i << "\n";
		//SqrRoot(squareRt, n);
    }
 
    // This condition is to handle the case whien n is a prime number
    // greater than 2
    if (n > 2)
	{
		//cout << n << " ";
		curMax = 999999;
	}
        
	//cout << "\n";
	
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


Vec< Pair< ZZ_pX, long > > checkA(ZZ_pX& base, ZZ_pX& currentPoly, ZZ_pXModulus& m, ZZ_p& leadCoeff, long& aSmoothness)
{
	//Initialize result of PowerMod
	ZZ_pX result;
	ZZ_pX monicResult;
	Vec< Pair< ZZ_pX, long > > factors;
	
	//Increase exponent by 1
	result = currentPoly * base;
	
	leadCoeff = LeadCoeff(result);
	ZZ lc;
	conv(lc, leadCoeff);
	
	//Factor leading coefficient before factoring polynomials
	aSmoothness = printFactors(lc, maxSmoothness);
	if (aSmoothness >= maxSmoothness)
	{
		return factors;
	}
	cout << aSmoothness << endl;
	cout << "Found a suitable leading coefficient: " << lc << endl;
	div(monicResult, result, leadCoeff);
	
	//cout << result << "\n";
	//cout << monicResult << "\n";
	
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

int main(int argc, char* argv[])
{
	ZZ startAt;
	if (argc < 2)
	{
		startAt = 0;
	}
	else
	{
		ZZ A(NTL::INIT_VAL, argv[1]);
		startAt = A;
	}
	
	
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
	
	ZZ firstA = getFirstA();
	currentA = firstA + startAt;
	cout << currentA << endl;
	
	//Using first A, do PowerMod
	ZZ_pX currentPoly;
	
	PowerMod(currentPoly, base, currentA, precomp);
	
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
		if (currentA % 10000 == 0)
			cout << "Current A value: " << currentA - firstA << endl;
		
		long result;
		factors = checkA(base, currentPoly, precomp, leadCoeff, result);
		
		
		if (checkFactors(factors))
		{
			
			
			//Check to see if the "b" values are below the current max	
			ZZ lc;
			conv(lc, leadCoeff);			
			bool wasGreater = false;
			for (int i = 0; i < factors.length(); i++)
			{
				ZZ_p coeff;
				GetCoeff(coeff, factors[i].a, 0);
				conv(lc, coeff);
				if (lc >= maxSmoothness)
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
			
			//Factor the leading coefficient. (now done before factoring polynomial)
			
			// result = printFactors(lc, maxSmoothness);
			// if (result >= maxSmoothness)
			// {
				// currentA++;
				// continue;
			// }
			
			//If we made it to here, new match found!
			//Adjust max, then print out the result.
			maxSmoothness = result;
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