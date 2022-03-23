#include <iostream>
#include <math.h>
using namespace std;

enum NumOfRoots{Zero, One, Two};

double* solver(double a, double b, double c, NumOfRoots& n) {
	double d = b * b - 2 * a * c;
	if (d > 0) {
		double* x = new double[2];
		x[0] = -b + sqrt(d) / (2 * a);
		x[1] = -b - sqrt(d) / (2 * a);
		n = Two;
		return x;
	}
	else if (d == 0) {
		double* x = new double(-b / (2 * a));
		n = One;
		return x;
	}
	else {
		n = Zero;
		return nullptr;
	}
}