#include <iostream>
#include <math.h>
using namespace std;

enum class NumOfRoots{Zero, One, Two};

double* solver(double a, double b, double c, NumOfRoots& n) {
	double d = b * b - 2 * a * c;
	if (d > 0) {
		double* x = new double[2];
		x[0] = (-b + sqrt(d)) / (2 * a);
		x[1] = (-b - sqrt(d)) / (2 * a);
		n = NumOfRoots::Two;
		return x;
	}
	else if (d == 0) {
		double* x = new double(-b / (2 * a));
		n = NumOfRoots::One;
		return x;
	}
	else {
		n = NumOfRoots::Zero;
		return nullptr;
	}
}

int main(void) {
	double a, b, c;
	std::cout << "Enter coefficient a: " << std::endl;
	std::cin >> a;
	std::cout << "Enter coefficient b: " << std::endl;
	std::cin >> b;
	std::cout << "Enter coefficient c: " << std::endl;
	std::cin >> c;
	std::cout << "Your equation is " << a << "x^2 + " << b << "x + " << c << std::endl;

	NumOfRoots n{};
	double* roots{ solver(a, b, c, n) };

	if (n == NumOfRoots::Zero) { std::cout << "\nEquation has no roots!\n" << std::endl; }
	if (n == NumOfRoots::One) { std::cout << "\nEquation has one root: " << roots[0] << std::endl; }
	if (n == NumOfRoots::Two) { std::cout << "\nEquation has two roots: " << roots[0] << " and " << roots[1] << std::endl; }

	delete[] roots;
}
