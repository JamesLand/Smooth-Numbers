#include <iostream>
#include <math.h>
#include <NTL/ZZ.h>
#include <NTL/ZZ_pXFactoring.h>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;
using namespace NTL;

ZZ currentA;

string threshStr("8375839430921074837583433589000");
ZZ thresh(INIT_VAL, threshStr.c_str());

ZZ maxSmoothness(0);
int maxBitSmoothness = 200;

//Algorithm shamelessly stolen from
//http://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/
//because i was too lazy to write this myself
//Modifications were made, though.
ZZ printFactors(ZZ n)
{
	ZZ curMax;
	cout << "Starting to factor lead coefficient" << endl;
	while (n % 2 == 0)
	{
		//cout << "2 ";
		n = n / 2;
		curMax = 2;
	}
	ZZ currentPrime = to_ZZ(3);
	while (NumBits(currentPrime) <= maxBitSmoothness)
	{
		//cout << currentPrime << " ";
		ZZ res;
		//cout << n % currentPrime << endl;
		while (n % currentPrime == to_ZZ(0))
		{
			//cout << currentPrime << " ";
			div(n, n, currentPrime);
			curMax = currentPrime;
		}
		NextPrime(currentPrime, currentPrime + 1);
	}

	cout << "Done factoring." << endl;
	// This condition is to handle the case whien n is a prime number
	// greater than 2
	if (n > 2)
	{
		//cout << n << " ";
		curMax = n;
	}

	//cout << "\n";

	return n;
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


Vec< Pair< ZZ_pX, long > > checkA(ZZ_pX& base, ZZ_pX& currentPoly, ZZ_pXModulus& m, ZZ_p& leadCoeff)
{
	//Initialize result of PowerMod
	ZZ_pX result;
	ZZ_pX monicResult;
	Vec< Pair< ZZ_pX, long > > factors;

	//Increase exponent by 1
	MulMod(result, currentPoly, base, m);
	currentPoly = result;

	leadCoeff = LeadCoeff(result);
	ZZ lc;
	conv(lc, leadCoeff);

	// if (lc > thresh)
	// {
	// return factors;
	// }
	// cout << "Number passed thresh" << endl;
	//Factor leading coefficient before factoring polynomials
	// aSmoothness = printFactors(lc, maxSmoothness);
	// if (aSmoothness >= maxSmoothness)
	// {
	// return factors;
	// }
	// cout << aSmoothness << endl;
	// cout << "Found a suitable leading coefficient: " << lc << endl;
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

	Vec< Pair< ZZ_pX, long > > factors;
	ZZ_p leadCoeff;
	currentA++;
	while (true)
	{
		if (currentA % 10000 == 0)
			cout << "Current A value: " << currentA - firstA << endl;

		ZZ result(-1);
		factors = checkA(base, currentPoly, precomp, leadCoeff);


		if (checkFactors(factors))
		{
			//cout << "Polynomial factors." << endl;

			//Check to see if the "b" values are below the current max	
			ZZ lc;
			conv(lc, leadCoeff);
			bool wasGreater = false;
			for (int i = 0; i < factors.length(); i++)
			{
				ZZ_p coeff;
				GetCoeff(coeff, factors[i].a, 0);
				conv(lc, coeff);
				if (NumBits(lc) >= maxBitSmoothness)
				{
					wasGreater = true;
					break;
				}
				else if (result < 0)
					result = lc;
				else if (lc > result)
					result = lc;
			}
			if (wasGreater)
			{
				currentA++;
				continue;
			}
			cout << result << endl;
			cout << "Found a suitable factoring polynomial: " << lc << endl;
			
			//Factor the leading coefficient. (now done before factoring polynomial)
			result = printFactors(lc);
			if (NumBits(result) >= maxBitSmoothness)
			{
				currentA++;
				continue;
			}
			
			
			cout << result << endl;
			maxBitSmoothness = NumBits(result);
			maxSmoothness = result;
			

			//If we made it to here, new match found!
			//Adjust max, then print out the result.
			ofstream myfile;
			myfile.open("log.txt");
			myfile << "--------------MATCH FOUND----------------------\n";
			myfile << currentA << endl;
			myfile << leadCoeff << endl;
			myfile << factors << endl;
			myfile << maxBitSmoothness << endl;
			myfile << "-----------------------------------------------\n";
			myfile.close();
			cout << "--------------MATCH FOUND----------------------\n";
			cout << currentA << endl;
			cout << leadCoeff << endl;
			cout << factors << endl;
			cout << maxBitSmoothness << endl;
			cout << "-----------------------------------------------\n";
		}
		currentA++;
	}

	//Check if factors are in the right form
	//cout << leadCoeff << endl;
	//cout << factors << endl;
	//cout << checkFactors(factors) << endl;
	//cout << endl;
	//ZZ lc;
	//conv(lc, leadCoeff);
	//printFactors(lc);
	//for (int i = 0; i < factors.length(); i++)
	//{
	//	ZZ_p coeff;
	//	GetCoeff(coeff, factors[i].a, 0);
	//	conv(lc, coeff);
	//	printFactors(lc, 200);
	//}

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